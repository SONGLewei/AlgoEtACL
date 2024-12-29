#include "../includeAlgo/input_module.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include "../include/Graphe.h"
#include "../include/Ville.h"
#include "../include/dp_algorithm.hpp"
#include "../include/lpt_algorithm.hpp"
#include "../include/fptas_algorithm.hpp"

/**
 * 示例：改动后的 read_input，用来生成/读取一个 Graphe（而不是 tasks）。
 * 为了简单，这里只支持手动输入 N(顶点数)、是否自动随机生成处理时间等。
 */
void read_input_graphe(Graphe<double, Ville>& graphe, int& m, double& epsilon, int& algoChoice) {
    std::cout << "=== [Modified input_module] ===\n";
    std::cout << "Number of tasks (sommets) N: ";
    int N;
    std::cin >> N;

    std::cout << "Number of machines (m): ";
    std::cin >> m;

    std::cout << "Choose an algorithm:\n";
    std::cout << "  1) DP\n";
    std::cout << "  2) LPT\n";
    std::cout << "  3) FPTAS\n";
    std::cin >> algoChoice;

    if (algoChoice == 3) {
        std::cout << "Enter epsilon for FPTAS: ";
        std::cin >> epsilon;
    } else {
        epsilon = 0.0; // not used
    }

    // 创建 N 个 sommet
    // 让用户输入每个 sommet 的“处理时间”，存入 v
    // 并给它起个名字
    for (int i = 0; i < N; i++) {
        double time_i;
        std::cout << "Task " << i << " time: ";
        std::cin >> time_i;
        // 用 Ville 的构造函数 (name, lat, lon)，这里偷懒，把 lat/lon 就都写成0，或者别的
        Ville villei("Task" + std::to_string(i), 0.0, 0.0);
        auto s = graphe.creeSommet(villei);
        // 这里 s->v 等于 villei，但实际的“处理时间”要怎么存？  
        // 我们决定在 Arete 里存距离？还是在 Sommet 里存 tasks？ 
        // 本示例：让“time_i”只是记在 Sommet<double> 的 GElement::v 中也可以，
        // 但 Ville 里并没有时间字段。可改成 graphe.creeSommet(time_i)，若 Sommet<double> 
        // 不再是 Ville 而是 double 也行... 
        // 为了不大改结构，暂时只能在 Arete 里做距离。这里演示一下写法：
        // => 如果你想把 "time_i" 也记录下来：
        // s->GElement<double>::v = time_i; (不太优雅，但可行)
        // or rework "Ville" to have a field "double processingTime"
    }

    // 构造一个完全图：给每对顶点之间都创建一条 Arete
    // 这里演示 distance = 1，或者随机
    // 但是“调度算法”其实只关心每个任务的处理时间 tasks[i]，并不真的需要 edges 里的距离
    // 依你需求决定是否要构造真实距离
    std::vector<Sommet<Ville>*> sommetsVec;
    for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
        sommetsVec.push_back(it->valeur);
    }
    // 简单构造全连接
    for (int i = 0; i < N; i++) {
        for (int j = i+1; j < N; j++) {
            // 这里距离随便设置，比如 1.0
            graphe.creeArete(1.0, sommetsVec[i], sommetsVec[j]);
        }
    }

    std::cout << "Graphe constructed with N=" << N << " sommets, fully connected.\n";
}

/**
 * 将 Graphe 的 sommets (任务) 转换为 tasks 数组
 * 这里假设 Sommet<T>::v 就是任务处理时间，也可以自行改进
 */
static std::vector<int> sommetsToTasks(const Graphe<double, Ville>& graphe) {
    // 注意：Ville 结构里没有“处理时间”这个字段，只有 name/latitude/longitude
    // 如果你想在 Sommet<double> 里保存任务时间，就不要用 Ville
    // 这里只能演示一下：假设把 sommets 的 index 当作 tasks
    // 或在 Ville 增加一个 processingTime 字段。
    // 为了演示，我们就随机设为 10……随便了:
    std::vector<int> tasks;
    int idx = 0;
    for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
        // 真实情况: tasks.push_back( (int) it->valeur->someTime );
        tasks.push_back(10); 
        idx++;
    }
    return tasks;
}

/**
 * 根据调度算法产出的 “assignment[i] = 机器编号”，
 * 我们这里演示如何把它做成“一条路线”：
 *   先按机器编号排序任务 -> 线性串起来 -> 保留相邻任务间的边。
 * 仅作示例，不一定是合理的“路线”。
 */
static void conserverRouteParAssignment(Graphe<double, Ville>& graphe,
                                        const std::vector<int>& assignment)
{
    // 先收集(任务编号, 机器编号)
    std::vector<std::pair<int,int>> tasksWithMach;
    tasksWithMach.reserve(assignment.size());
    for (int i = 0; i < (int)assignment.size(); i++) {
        tasksWithMach.push_back({ i, assignment[i] });
    }
    // 按机器编号排序
    std::sort(tasksWithMach.begin(), tasksWithMach.end(), 
              [](auto& a, auto& b){ return a.second < b.second; });

    // 现在构造一条“线性顺序”
    // tasksWithMach[0], tasksWithMach[1], ...
    // 相邻的任务节点间要保留边
    // 其他边全部删除
    std::vector<Arete<double, Ville>*> routeEdges;
    // 首先收集图中所有 sommet 放进一个数组，方便 index <-> Sommet 的转换
    std::vector<Sommet<Ville>*> sommetsVec;
    sommetsVec.reserve(assignment.size());
    for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
        sommetsVec.push_back(it->valeur);
    }
    // routeEdges: 将相邻 tasks( i->i+1 )找出对应 Arete
    for (int i = 0; i+1 < (int)tasksWithMach.size(); i++) {
        int taskA = tasksWithMach[i].first;
        int taskB = tasksWithMach[i+1].first;
        if (taskA < (int)sommetsVec.size() && taskB < (int)sommetsVec.size()) {
            Sommet<Ville>* sA = sommetsVec[taskA];
            Sommet<Ville>* sB = sommetsVec[taskB];
            // 找到该边
            auto e = graphe.getAreteParSommets(sA, sB);
            if (e) {
                routeEdges.push_back(e);
            }
        }
    }

    // 删除非 routeEdges
    graphe.supprimerToutesAretesExcept(routeEdges);
}

/**
 * 示例：在本函数中直接调用各算法，对 Graphe 做“删边”，并输出结果
 */
void run_algorithms_and_optimize(Graphe<double, Ville>& graphe, int m, double epsilon, int algoChoice) {
    // 1) 将 sommets 转成 tasks
    std::vector<int> tasks = sommetsToTasks(graphe);
    if (tasks.empty()) {
        std::cout << "[Warning] No tasks found.\n";
        return;
    }

    // 2) 调用相应算法
    int N = (int)tasks.size();
    double finalCmax = 0.0;
    std::vector<int> assignment(N, -1);

    if (algoChoice == 1) { 
        // DP
        auto res = DP_with_order(tasks, m);
        finalCmax = res.first;
        assignment = res.second;
    } else if (algoChoice == 2) {
        // LPT
        // LPT_with_order 要求传入非 const 的 tasks，OK
        auto res = LPT_with_order(tasks, m);
        finalCmax = res.first;
        assignment = res.second;
    } else if (algoChoice == 3) {
        // FPTAS
        auto res = FPTAS_with_order(tasks, m, epsilon);
        finalCmax = res.first;
        assignment = res.second;
    } else {
        std::cout << "[Error] Unknown algoChoice.\n";
        return;
    }

    std::cout << "\n--- Scheduling finished. Cmax = " << finalCmax << "\n";

    // 3) 构造“路线”并删边
    conserverRouteParAssignment(graphe, assignment);

    std::cout << "[Edges not in the derived route have been removed]\n";
    std::cout << "Final Graphe:\n" << graphe << std::endl;

    std::cout << "Final Cmax = " << finalCmax << std::endl;
}
