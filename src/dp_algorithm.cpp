#include "../include/dp_algorithm.hpp"
#include <iostream>
#include <climits>
#include <algorithm>
#include <vector>

/**
 * @brief 计算子集 subset（以位掩码形式）在 tasks 数组中的负载和
 */
static int subset_sum(const std::vector<int>& tasks, int subset) {
    int sum = 0;
    for (int i = 0; i < (int)tasks.size(); i++) {
        if (subset & (1 << i)) {
            sum += tasks[i];
        }
    }
    return sum;
}

/**
 * 原有的 DP，返回最优 Cmax
 */
int DP(const std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        std::cout << "DP: Invalid input parameters." << std::endl;
        return 0;
    }

    int total_subsets = 1 << N; // 2^N
    
    // dp[S][j]：子集 S 分配在 j 台机器时的最优负载(最小化最大完成时间)
    std::vector<std::vector<int>> dp(total_subsets, std::vector<int>(m + 1, INT_MAX));

    // 初始化：只用 1 台机器时，对子集 S 的负载就是 subset_sum(S)
    for (int S = 0; S < total_subsets; S++) {
        dp[S][1] = subset_sum(tasks, S);
    }
    dp[0][1] = 0;

    // 逐台增加机器数量
    for (int j = 2; j <= m; j++) {
        for (int S = 0; S < total_subsets; S++) {
            for (int T = S; T > 0; T = (T - 1) & S) {
                int remaining = S & (~T);
                int current_load = subset_sum(tasks, remaining);
                int candidate = std::max(dp[T][j - 1], current_load);
                if (candidate < dp[S][j]) {
                    dp[S][j] = candidate;
                }
            }
        }
    }

    int result = dp[total_subsets - 1][m];
    std::cout << "DP scheduling completed. Minimum Cmax: " << result << std::endl;
    return result;
}

/**
 * 新增版本：返回 (Cmax, 调度顺序)。
 * 这里为了演示，我们仅返回每个任务所分配的机器编号。
 * 注：要回溯 dp 数组才能得到机器分配情况，这里做一个简单示例。
 */
std::pair<int, std::vector<int>> DP_with_order(const std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        return {0, {}};
    }

    int total_subsets = 1 << N; 
    std::vector<std::vector<int>> dp(total_subsets, std::vector<int>(m + 1, INT_MAX));
    // prev[S][j] 用来记录本次划分时，子集 S 是由哪一个 T 拆分而来
    // 这样可回溯出具体分配
    std::vector<std::vector<int>> prev(total_subsets, std::vector<int>(m + 1, -1));

    // 初始化
    for (int S = 0; S < total_subsets; S++) {
        dp[S][1] = subset_sum(tasks, S);
    }
    dp[0][1] = 0;

    for (int j = 2; j <= m; j++) {
        for (int S = 0; S < total_subsets; S++) {
            for (int T = S; T > 0; T = (T - 1) & S) {
                int remaining = S & (~T);
                int current_load = subset_sum(tasks, remaining);
                int candidate = std::max(dp[T][j - 1], current_load);
                if (candidate < dp[S][j]) {
                    dp[S][j] = candidate;
                    prev[S][j] = T;
                }
            }
        }
    }

    int bestCmax = dp[total_subsets - 1][m];

    // 回溯，构造一个“任务->机器编号”的分配
    std::vector<int> assignment(N, -1);
    // 我们从后往前回溯
    int curS = total_subsets - 1;
    int curM = m;
    while (curM > 0 && curS > 0) {
        if (curM == 1) {
            // 剩余所有任务都在子集 curS
            // 这意味着它们都被分配在机器 (curM-1)
            for (int i = 0; i < N; i++) {
                if ((curS & (1 << i)) != 0) {
                    assignment[i] = curM - 1; 
                }
            }
            break;
        } else {
            int T = prev[curS][curM];
            if (T < 0) {
                // 无法回溯
                break;
            }
            // T 子集在前 (curM-1) 个机器, curS\T 在第 curM 台机器
            int subsetDiff = curS & (~T);
            // 给 subsetDiff 中的任务标记 curM-1
            for (int i = 0; i < N; i++) {
                if ((subsetDiff & (1 << i)) != 0) {
                    assignment[i] = curM - 1;
                }
            }
            curS = T;
            curM--;
        }
    }

    return {bestCmax, assignment};
}
