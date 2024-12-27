#pragma once
#include <vector>

/**
 * @brief 动态规划调度算法 (DP)。计算完成时间 Cmax 的最优值。
 * @param tasks  各任务的负载（处理时间）
 * @param m      机器数量
 * @return       计算得到的最小 Cmax
 */
int DP(const std::vector<int>& tasks, int m);
