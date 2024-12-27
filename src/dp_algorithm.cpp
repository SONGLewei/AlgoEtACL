#include "../include/dp_algorithm.hpp"
#include <iostream>
#include <climits>
#include <algorithm>

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
 * 动态规划调度算法 (DP) 的实现。
 * 逻辑等价于原 C 版本，但改成使用 C++ 容器与 I/O。
 */
int DP(const std::vector<int>& tasks, int m) {
    int N = (int)tasks.size();
    if (N == 0 || m <= 0) {
        std::cout << "DP: Invalid input parameters." << std::endl;
        return 0;
    }

    int total_subsets = 1 << N; // 2^N 个子集
    
    // dp[S][j]：子集 S 分配在 j 台机器时的最优负载(最小化最大完成时间)
    // 这里使用一个二维的 std::vector 来存储
    std::vector<std::vector<int>> dp(total_subsets, std::vector<int>(m + 1, INT_MAX));

    // 初始化：只用 1 台机器时，对子集 S 的负载就是 subset_sum(S)
    for (int S = 0; S < total_subsets; S++) {
        dp[S][1] = subset_sum(tasks, S);
    }
    dp[0][1] = 0; // 空集负载为 0

    // 逐台增加机器数量
    for (int j = 2; j <= m; j++) {
        for (int S = 0; S < total_subsets; S++) {
            // 枚举 S 的所有非空子集 T
            // T 是 S 的子集 => T & S = T
            // 枚举方式：T = (T - 1) & S
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

    // dp[total_subsets - 1][m] 即所有任务分配到 m 台机器后的最优值
    int result = dp[total_subsets - 1][m];
    std::cout << "DP scheduling completed. Minimum Cmax: " << result << std::endl;
    return result;
}
