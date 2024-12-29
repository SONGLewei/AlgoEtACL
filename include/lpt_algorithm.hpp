#pragma once

#include <vector>
#include <utility>

/**
 * @brief 原有 LPT，返回 Cmax
 */
int LPT(std::vector<int>& tasks, int m);

/**
 * @brief 新增：LPT 调度并返回 (Cmax, 调度顺序)。
 *        这里“顺序”同样是指每个任务分配到哪台机器（或可以改成纯任务序列）。
 */
std::pair<int, std::vector<int>> LPT_with_order(std::vector<int>& tasks, int m);
