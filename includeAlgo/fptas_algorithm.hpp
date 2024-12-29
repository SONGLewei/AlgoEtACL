#pragma once

#include <vector>
#include <utility>

/**
 * 原有 FPTAS，返回近似 Cmax
 */
double FPTAS(const std::vector<int>& tasks, int m, double epsilon);

/**
 * 新增：FPTAS_with_order，返回 (近似Cmax, 分配方案)
 */
std::pair<double, std::vector<int>> FPTAS_with_order(const std::vector<int>& tasks,
                                                     int m,
                                                     double epsilon);
