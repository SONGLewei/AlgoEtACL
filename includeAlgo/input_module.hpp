#pragma once

#include <vector>
#include <utility>
#include "../includeGraphe/Graphe.h"
#include "../includeGraphe/Ville.h"

/**
 * 之前可能就有的函数 (如你原先的逻辑),
 * 如果不再需要，可以删除或注释
 */
void generate_random_tasks(std::vector<int>& tasks,
                           std::vector<std::vector<int>>& compatibility,
                           int& N,
                           int& m,
                           int max_time);

void read_input(std::vector<int>& tasks,
                std::vector<std::vector<int>>& compatibility,
                int& N,
                int& m);

/**
 * 下列是我们新增/修改的函数声明，用于直接操作 Graphe
 */

/**
 * @brief 让用户交互式创建一个完整图 (或你可以进行别的构造方式)
 *        - 会询问用户 N, m, 选择的算法( algoChoice ), 以及 FPTAS的 epsilon(若algoChoice=3)
 *        - 并为每个节点输入“处理时间”（仅示意）
 */
void read_input_graphe(Graphe<double, Ville>& graphe, int& m, double& epsilon, int& algoChoice);

/**
 * @brief 将接收的图进行调度优化：
 *        1) 转换成 tasks
 *        2) 调用所选算法 (DP / LPT / FPTAS)
 *        3) 根据分配结果删除与最终路线无关的边
 *        4) 输出最终图和 Cmax
 */
void run_algorithms_and_optimize(Graphe<double, Ville>& graphe,
                                 int m,
                                 double epsilon,
                                 int algoChoice);

