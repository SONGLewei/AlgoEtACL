#pragma once
#include <vector>

/**
 * @brief 基于图着色的简单“排程”思路示例
 * @param tasks             各任务处理时间
 * @param incompatibility   不兼容矩阵，incompatibility[i][j] = 1 表示任务 i 与 j 不兼容
 */
void graph_coloring(const std::vector<int>& tasks,
                    const std::vector<std::vector<int>>& incompatibility);
