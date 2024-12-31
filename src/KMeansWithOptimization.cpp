#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeGraphe/KMeans.h"               // 需要 kMeansClustering(...) 的声明和实现
#include "../includeAlgo/fptas_algorithm.hpp" // FPTAS 与 FPTAS_with_order
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include <cmath>
#include <unordered_set>
#include <algorithm>

Graphe<double, Ville> KMeansWithOptimization::optimizeAndReturnGraph(
    const Graphe<double, Ville>& graphe,
    double epsilon,
    double improvementThreshold
) {
    // 初始聚类数从K=1开始
    int K = 1;
    double prevCmax = std::numeric_limits<double>::max();
    Graphe<double, Ville> bestGraph;

    // 迭代上限
    const int MAX_ITERATIONS = 100;
    int iterationCount = 0;

    // 输出结果文件（可选）
    std::ofstream resultFile("result.txt");
    if (!resultFile.is_open()) {
        std::cerr << "Failed to open result file for writing." << std::endl;
        // 如果打不开，继续执行也行，只是不会写文件
    }

    // 开始多轮迭代
    while (true) {
        iterationCount++;
        auto start = std::chrono::high_resolution_clock::now();

        // 1. 收集所有 Ville
        std::vector<Ville> villes;
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            villes.push_back(it->valeur->v);
        }

        // 2. 做KMeans，选最优聚类
        auto bestClusters = std::vector<std::vector<Ville>>();
        double bestCmaxLocal = std::numeric_limits<double>::max();

        // 做 10 次随机KMeans，然后选择负载最小的那个结果
        for (int i = 0; i < 10; ++i) {
            auto tempClusters = kMeansClustering(villes, K);
            double tempCmax = calculateMaxLoad(tempClusters, graphe);
            if (tempCmax < bestCmaxLocal) {
                bestClusters = tempClusters;
                bestCmaxLocal = tempCmax;
            }
        }

        // 这里 bestClusters 就是本轮最好的 KMeans 结果
        // bestCmaxLocal 是它的(未平衡)最大负载

        // 3. 从聚类生成若干子图，并计算负载
        std::vector<Graphe<double, Ville>> clusterGraphs;
        std::vector<int> clusterLoads;
        for (const auto& cluster : bestClusters) {
            std::unordered_set<std::string> cityNames;
            for (const auto& ville : cluster) {
                cityNames.insert(ville.name);
            }
            Graphe<double, Ville> clusterGraph = graphe.extractSubgraph(cityNames);
            clusterGraphs.push_back(clusterGraph);

            int load = calculateClusterLoad(clusterGraph);
            clusterLoads.push_back(load);
        }

        // 4. 用FPTAS平衡负载
        std::pair<double, std::vector<int>> balancedResult = 
            balanceClustersFPTAS(clusterLoads, K, epsilon);

        // ========== 改动之处：不再 break，而是 fallback ==========
        double currentCmax = 0.0;
        if (balancedResult.first < 0) {
            // FPTAS 失败 => fallback
            std::cerr << "FPTAS balancing failed, fallback to unbalanced load." << std::endl;
            resultFile << "FPTAS balancing failed, fallback to unbalanced load.\n";
            // 用本次KMeans聚类得到的最大负载 bestCmaxLocal 作为近似Cmax
            currentCmax = bestCmaxLocal;
        } else {
            currentCmax = balancedResult.first;
        }

        // 5. 计算改进率
        double improvement = (prevCmax - currentCmax) / prevCmax;
        double absoluteChange = std::fabs(prevCmax - currentCmax);

        // 日志输出
        std::cout << "Previous Cmax: " << prevCmax
                  << ", Current Cmax: " << currentCmax << "\n";
        resultFile << "Previous Cmax: " << prevCmax
                   << ", Current Cmax: " << currentCmax << "\n";

        std::cout << "Iteration with K = " << K 
                  << ", Approx Cmax: " << currentCmax << "\n";
        resultFile << "Iteration with K = " << K
                   << ", Approx Cmax: " << currentCmax << "\n";

        std::cout << "Improvement from previous Cmax: " 
                  << (improvement * 100) << "%\n";
        resultFile << "Improvement from previous Cmax: "
                   << (improvement * 100) << "%\n";

        std::cout << "Absolute change in Cmax: " << absoluteChange << "\n";
        resultFile << "Absolute change in Cmax: " << absoluteChange << "\n";

        // 打印本轮聚类信息
        for (size_t i = 0; i < clusterGraphs.size(); ++i) {
            std::cout << "Cluster " << (i + 1) << ": ";
            resultFile << "Cluster " << (i + 1) << ": ";

            // 输出城市列表
            for (auto it = clusterGraphs[i].lSommets; it != nullptr; it = it->suivant) {
                std::cout << it->valeur->v.name << " ";
                resultFile << it->valeur->v.name << " ";
            }
            std::cout << "Load: " << clusterLoads[i] << "\n";
            resultFile << "Load: " << clusterLoads[i] << "\n";
        }

        // 6. 判断是否停止
        //    如果改进率小于一定阈值并且绝对变化也很小，或者超过迭代上限，则停止
        if ((improvement < improvementThreshold && absoluteChange < 1e-3) 
             || iterationCount >= MAX_ITERATIONS)
        {
            std::cout << "Stopping optimization:\n";
            resultFile << "Stopping optimization:\n";

            if (improvement < improvementThreshold) {
                std::cout << "  Improvement below threshold.\n";
                resultFile << "  Improvement below threshold.\n";
            }
            if (absoluteChange < 1e-3) {
                std::cout << "  Cmax change below min threshold.\n";
                resultFile << "  Cmax change below min threshold.\n";
            }
            if (iterationCount >= MAX_ITERATIONS) {
                std::cout << "  Reached max iterations.\n";
                resultFile << "  Reached max iterations.\n";
            }
            break;
        }

        // 7. 否则更新 prevCmax, bestGraph
        prevCmax = currentCmax;
        bestGraph = mergeClusters(clusterGraphs);

        // 8. 增加 K => 下一轮迭代
        K++;

        // 统计时间
        auto end = std::chrono::high_resolution_clock::now();
        double elapsedSec = std::chrono::duration<double>(end - start).count();
        std::cout << "Iteration time: " << elapsedSec << " seconds.\n";
        resultFile << "Iteration time: " << elapsedSec << " seconds.\n";
    }

    resultFile << "Final Cmax: " << prevCmax << "\n";
    resultFile.close();

    return bestGraph;
}

int KMeansWithOptimization::calculateClusterLoad(const Graphe<double, Ville>& cluster) {
    double totalLoad = 0.0;
    // 简化：把所有边的权值累加
    for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
        totalLoad += it->valeur->v; 
    }
    return static_cast<int>(totalLoad);
}

std::pair<double, std::vector<int>> 
KMeansWithOptimization::balanceClustersFPTAS(const std::vector<int>& clusterLoads,
                                             int numClusters,
                                             double epsilon) 
{
    // 这里使用 fptas_algorithm.hpp 中提供的 FPTAS_with_order
    return FPTAS_with_order(clusterLoads, numClusters, epsilon);
}

Graphe<double, Ville> 
KMeansWithOptimization::mergeClusters(const std::vector<Graphe<double, Ville>>& clusters) {
    Graphe<double, Ville> mergedGraph;
    // 简单合并所有子图的顶点和边
    for (const auto& cluster : clusters) {
        // 拷贝节点
        for (auto it = cluster.lSommets; it != nullptr; it = it->suivant) {
            mergedGraph.creeSommet(it->valeur->v);
        }
        // 拷贝边
        for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
            mergedGraph.creeArete(
                it->valeur->v,
                it->valeur->debut,
                it->valeur->fin
            );
        }
    }
    return mergedGraph;
}

double 
KMeansWithOptimization::calculateMaxLoad(const std::vector<std::vector<Ville>>& clusters,
                                         const Graphe<double, Ville>& graphe)
{
    double maxLoad = 0.0;
    // 对每个聚类：取出所有城市名 => 提取子图 => 计算负载 => 取最大值
    for (const auto& cluster : clusters) {
        std::unordered_set<std::string> cityNames;
        for (const auto& ville : cluster) {
            cityNames.insert(ville.name);
        }
        Graphe<double, Ville> subgraph = graphe.extractSubgraph(cityNames);
        double load = static_cast<double>(calculateClusterLoad(subgraph));
        if (load > maxLoad) {
            maxLoad = load;
        }
    }
    return maxLoad;
}