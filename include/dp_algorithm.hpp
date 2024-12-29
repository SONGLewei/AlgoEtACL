#pragma once

#include <vector>
#include <utility>

/**
 * @brief 原有 DP，返回最优的 Cmax
 */
int DP(const std::vector<int>& tasks, int m);

/**
 * @brief 新增：DP 调度，返回 (Cmax, 调度顺序)。
 *        调度顺序示例：返回一个长度为 N 的数组，其中第 i 个元素是任务 i 分配到的“机器编号”。
 *        你也可以修改为返回某种“任务排列”。
 */
std::pair<int, std::vector<int>> DP_with_order(const std::vector<int>& tasks, int m);
