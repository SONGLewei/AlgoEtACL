#pragma once
#include <vector>

/**
 * @brief LPT (Longest Processing Time first) 调度算法
 * @param tasks 任务处理时间
 * @param m     机器数量
 * @return      LPT 计算出的最大完成时间 Cmax
 */
int LPT(std::vector<int>& tasks, int m);
