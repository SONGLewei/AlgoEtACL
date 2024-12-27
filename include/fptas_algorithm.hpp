#pragma once
#include <vector>

/**
 * @brief 全多项式时间近似方案 (FPTAS) 调度算法
 * @param tasks    各任务的负载
 * @param m        机器数
 * @param epsilon  近似参数，越小越贴近最优，时间开销也越大
 * @return         近似的完成时间 Cmax
 */
double FPTAS(const std::vector<int>& tasks, int m, double epsilon);
