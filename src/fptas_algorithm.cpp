#include "../include/fptas_algorithm.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <cmath>

/**
 * @brief 计算子集 subset（位掩码）在 tasks 数组中的负载和 (用于 scaled tasks)
 */
static int subset_sum_scaled(const std::vector<int>& tasks, int subset) {
    int sum = 0;
    for (int i = 0; i < (int)tasks.size(); i++) {
        if (subset & (1 << i)) {
            sum += tasks[i];
        }
    }
    return sum;
}

/**
 * 全多项式时间近似方案 (FPTAS) 的实现
 * 和原 C 版逻辑等价，使用 C++ 风格。
 */
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

    // 计算推荐最小 epsilon
    double epsilon_min = (double)N / (double)p_max;
    std::cout << "Recommended minimum epsilon: " << epsilon_min << std::endl;

    if (epsilon < epsilon_min) {
        std::cout << "The provided approximation parameter (epsilon) is too small." << std::endl
                  << "Please choose epsilon >= " << epsilon_min << std::endl;
        return -1.0; // 表示无法进行
    }

    // 计算缩放因子 K
    double K = (epsilon * p_max) / N;
    std::cout << "With epsilon = " << epsilon << ", the scaling factor K = " << K << std::endl;

    // 缩放任务处理时间
    std::vector<int> scaled_tasks(N);
    for (int i = 0; i < N; i++) {
        int scaled = static_cast<int>(tasks[i] / K);
        // 确保至少为 1
        scaled_tasks[i] = (scaled < 1) ? 1 : scaled;
    }

    // 打印缩放后的任务
    std::cout << "Scaled tasks: ";
    for (auto v : scaled_tasks) {
        std::cout << v << " ";
    }
    std::cout << std::endl;

    int total_subsets = 1 << N;
    // dp_prev[S] 表示子集 S 在 j-1 台机器下的最优负载
    // dp_curr[S] 表示子集 S 在 j 台机器下的最优负载
    std::vector<int> dp_prev(total_subsets), dp_curr(total_subsets, INT_MAX);

    // 用 1 台机器时，dp_prev[S] = subset_sum_scaled(S)
    for (int S = 0; S < total_subsets; S++) {
        dp_prev[S] = subset_sum_scaled(scaled_tasks, S);
    }
    dp_prev[0] = 0; // 空集负载为 0

    for (int j = 2; j <= m; j++) {
        std::cout << "Processing machine " << j << "..." << std::endl;
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
        dp_prev = dp_curr; // 覆盖到上一层
    }

    // dp_prev[total_subsets - 1] 是缩放后的 Cmax
    int scaled_Cmax = dp_prev[total_subsets - 1];
    double Cmax = scaled_Cmax * K;
    std::cout << "FPTAS scheduling completed. Approximate Cmax: " << Cmax << std::endl;
    return Cmax;
}
