#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include "../include/Graphe.h"
#include "../include/Ville.h"
#include "../include/GrapheExporter.h"

// 这是我们在 input_module.cpp 中实现的头文件，
// 里头含有 read_input_graphe(...) / run_algorithms_and_optimize(...) 声明
#include "../include/input_module.hpp"

int main() {
    // 随机数种子，仅供演示
    std::srand((unsigned)std::time(nullptr));

    // -----------------------------
    // 1. 构造一个较复杂的“完整图”
    //    在这里，我们示例手动 / 随机地创建一个带 N 个顶点的完整图
    //    如果你想交互式输入，可直接调用 read_input_graphe(...) 即可
    // -----------------------------
    Graphe<double, Ville> graphe; // 我们要构造的图

    // 先问用户，是否要走“手动输入”(read_input_graphe)，还是“自动示例构造”
    std::cout << "Do you want to (1) manually input a graph, or (2) build a random sample? ";
    int buildChoice;
    std::cin >> buildChoice;

    int m = 0;
    double epsilon = 0.0;
    int algoChoice = 0;

    if (buildChoice == 1) {
        // 让用户交互输入
        // 这会在 input_module.cpp 里创建图，附带用户指定的 N, m, algoChoice, epsilon等
        read_input_graphe(graphe, m, epsilon, algoChoice);
    } else {
        // 自动示例：随机生成一个带 N=10 个节点的完整图
        int N = 10;  // 也可改大点
        m = 3;       // 默认机器数
        algoChoice = 2; // 默认：2 => LPT
        epsilon = 0.2;  // 如果需要用 FPTAS，可手动改

        // 随机给每个节点一个“处理时间”？ 
        // 因为 Ville 结构没有“processingTime”，这里仅当做演示
        // name = "Node_i", lat/lon = 0,0，边的权重我们都设置为1或随机都行。
        for (int i = 0; i < N; i++) {
            // 构造城市(只是演示；实际可能要另行存储任务处理时间)
            Ville v("Node" + std::to_string(i), 0.0, 0.0);
            graphe.creeSommet(v);
        }

        // 收集所有顶点
        std::vector<Sommet<Ville>*> allSommets;
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            allSommets.push_back(it->valeur);
        }

        // 完整图：给任意 i<j 的两点连一条边，权重这里都设置为(1~10)随机
        for (int i = 0; i < N; i++) {
            for (int j = i+1; j < N; j++) {
                double w = 1 + (std::rand() % 10); 
                graphe.creeArete(w, allSommets[i], allSommets[j]);
            }
        }

        std::cout << "[Random sample graph built with " << N 
                  << " nodes, fully connected. Default m=" << m 
                  << ", algoChoice=" << algoChoice 
                  << " (2=>LPT), epsilon=" << epsilon << "]\n\n";

        // 如果想让用户自己决定算法，可再加交互：
        // std::cout << "Please choose algorithm: 1)DP 2)LPT 3)FPTAS => ";
        // std::cin >> algoChoice;
        // if (algoChoice == 3) {
        //     std::cout << "epsilon? ";
        //     std::cin >> epsilon;
        // }
    }

    // 打印一下初始图概况
    std::cout << "Initial Graph:\n" << graphe << "\n";

    // -----------------------------
    // 2. 调用 run_algorithms_and_optimize(...)
    //    这个函数在 input_module.cpp 中，
    //    它会：把图映射成 tasks => 调度 => 获取分配 => 删边 => 输出Cmax
    // -----------------------------
    run_algorithms_and_optimize(graphe, m, epsilon, algoChoice);

    // -----------------------------
    // 3. 最终若要将结果导出 JSON (可选)
    // -----------------------------
    std::string exportFilename = "optimized_graph.json";
    GrapheExporter::exporter(graphe, exportFilename);
    std::cout << "Exported final graph to: " << exportFilename << std::endl;

    // 程序结束
    return 0;
}
