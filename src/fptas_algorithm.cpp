#include "../include/fptas_algorithm.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <cmath>
#include <vector>

static int subset_sum_scaled(const std::vector<int>& tasks, int subset) {
    int sum = 0;
    for (int i = 0; i < (int)tasks.size(); i++) {
        if (subset & (1 << i)) {
            sum += tasks[i];
        }
    }
    return sum;
}

double FPTAS(const std::vector<int>& tasks, int m, double epsilon) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0 || epsilon <= 0.0) {
        std::cout << "FPTAS: Invalid input parameters." << std::endl;
        return 0.0;
    }

    // 找到最大任务处理时间 p_max
    int p_max = 0;
    for (int t : tasks) {
        if (t > p_max) p_max = t;
    }

    double epsilon_min = (double)N / (double)p_max;
    std::cout << "Recommended minimum epsilon: " << epsilon_min << std::endl;
    if (epsilon < epsilon_min) {
        std::cout << "The provided epsilon is too small. Must >= " << epsilon_min << std::endl;
        return -1.0;
    }

    double K = (epsilon * p_max) / N;
    std::cout << "Scaling factor K = " << K << std::endl;

    std::vector<int> scaled_tasks(N);
    for (int i = 0; i < N; i++) {
        int scaled = (int)(tasks[i] / K);
        scaled_tasks[i] = (scaled < 1) ? 1 : scaled;
    }

    int total_subsets = 1 << N;
    std::vector<int> dp_prev(total_subsets), dp_curr(total_subsets, INT_MAX);

    for (int S = 0; S < total_subsets; S++) {
        dp_prev[S] = subset_sum_scaled(scaled_tasks, S);
    }
    dp_prev[0] = 0;

    for (int j = 2; j <= m; j++) {
        for (int S = 0; S < total_subsets; S++) {
            dp_curr[S] = INT_MAX;
            for (int T = S; T > 0; T = (T - 1) & S) {
                int remaining = S & (~T);
                int current_load = subset_sum_scaled(scaled_tasks, remaining);
                int candidate = std::max(dp_prev[T], current_load);
                if (candidate < dp_curr[S]) {
                    dp_curr[S] = candidate;
                }
            }
        }
        dp_prev = dp_curr;
    }

    int scaled_Cmax = dp_prev[total_subsets - 1];
    double Cmax = scaled_Cmax * K;
    std::cout << "FPTAS done. Approx Cmax: " << Cmax << std::endl;
    return Cmax;
}

/**
 * 新增：FPTAS_with_order
 * 同理，这里演示回溯分配方案。只做一个简化示例，详尽回溯略。
 */
std::pair<double, std::vector<int>> FPTAS_with_order(const std::vector<int>& tasks,
                                                     int m,
                                                     double epsilon)
{
    // 与上面 FPTAS 类似，这里省略重复的过程，只演示如何返回分配方案
    // 你可以自行仿照 DP_with_order 的思路去实现回溯

    double cmax = FPTAS(tasks, m, epsilon);
    if (cmax < 0) {
        // 表示无法进行
        return {cmax, {}};
    }

    // 为简化，这里直接返回一个空的 assignment（实际需回溯/记录）
    // 你可以自行改进以得到更准确的机器分配
    std::vector<int> dummyAssignment(tasks.size(), 0);
    return { cmax, dummyAssignment };
}
