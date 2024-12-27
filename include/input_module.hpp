#pragma once
#include <vector>

/**
 * @brief 从用户或随机方式获取任务信息、机器数、兼容矩阵等
 *
 * @param tasks         [out] 输出的任务处理时间列表
 * @param compatibility [out] 输出的兼容矩阵 (N*N)
 * @param N             [out] 任务个数
 * @param m             [out] 机器数量
 *
 * 也可以把所有这些返回值封装到一个结构体中返回，这里为演示保留原函数风格。
 */
void read_input(std::vector<int>& tasks,
                std::vector<std::vector<int>>& compatibility,
                int& N,
                int& m);

/**
 * @brief 生成随机任务和兼容矩阵
 * @param tasks         [out] 生成的任务
 * @param compatibility [out] 生成的兼容矩阵
 * @param N             任务数
 * @param m             机器数
 * @param max_time      任务最大处理时间
 */
void generate_random_tasks(std::vector<int>& tasks,
                           std::vector<std::vector<int>>& compatibility,
                           int& N,
                           int& m,
                           int max_time);
