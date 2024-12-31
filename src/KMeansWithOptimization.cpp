#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeGraphe/KMeans.h"               // kMeansClustering 函数在此定义
#include "../includeAlgo/fptas_algorithm.hpp" // FPTAS 与 FPTAS_with_order 在这里
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
    // 初始聚类数 K = 1
    int K = 1;
    double prevCmax = std::numeric_limits<double>::max();
    Graphe<double, Ville> bestGraph;

    const int MAX_ITERATIONS = 100;
    int iterationCount = 0;

    // 输出结果到文件（可选）
    std::ofstream resultFile("result.txt");
    if (!resultFile.is_open()) {
        std::cerr << "Failed to open result file." << std::endl;
        return Graphe<double, Ville>();
    }

    // 开始迭代：逐步增大 K
    while (true) {
        iterationCount++;
        auto start = std::chrono::high_resolution_clock::now();

        // 1. 收集图中所有 Ville
        std::vector<Ville> villes;
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            villes.push_back(it->valeur->v);
        }

        // 2. 多次随机初始，选择最优的KMeans结果
        auto bestClusters = std::vector<std::vector<Ville>>();
        double bestCmaxLocal = std::numeric_limits<double>::max();

        // 做 10 次随机 KMeans，然后选择负载最小的那一次
        for (int i = 0; i < 10; ++i) {
            auto tempClusters = kMeansClustering(villes, K);
            double tempCmax = calculateMaxLoad(tempClusters, graphe);
            if (tempCmax < bestCmaxLocal) {
                bestClusters = tempClusters;
                bestCmaxLocal = tempCmax;
            }
        }

        // 这里 bestClusters 就是我们在本轮迭代下，K固定时找到的最好聚类
        auto clusters = bestClusters;

        // 3. 从每个聚类构造子图，并计算负载
        std::vector<Graphe<double, Ville>> clusterGraphs;
        std::vector<int> clusterLoads;

        for (const auto& cluster : clusters) {
            // 将该聚类的所有城市名字放到 set 里
            std::unordered_set<std::string> clusterCityNames;
            for (const auto& ville : cluster) {
                clusterCityNames.insert(ville.name);
            }
            // 根据城市名提取子图
            Graphe<double, Ville> clusterGraph = graphe.extractSubgraph(clusterCityNames);
            clusterGraphs.push_back(clusterGraph);
            clusterLoads.push_back(calculateClusterLoad(clusterGraph));
        }

        // 4. 用FPTAS平衡所有聚类负载
        auto balancedResult = balanceClustersFPTAS(clusterLoads, K, epsilon);
        if (balancedResult.first < 0) {
            std::cerr << "FPTAS balancing failed." << std::endl;
            break;
        }

        double currentCmax = balancedResult.first;
        double improvement = (prevCmax - currentCmax) / prevCmax;
        double absoluteChange = std::fabs(prevCmax - currentCmax);

        // 打印一些调试/观测信息
        std::cout << "Previous Cmax: " << prevCmax 
                  << ", Current Cmax: " << currentCmax << std::endl;
        resultFile << "Previous Cmax: " << prevCmax 
                   << ", Current Cmax: " << currentCmax << std::endl;

        std::cout << "Iteration with K = " << K 
                  << ", Approx Cmax: " << currentCmax << std::endl;
        resultFile << "Iteration with K = " << K 
                   << ", Approx Cmax: " << currentCmax << std::endl;

        std::cout << "Improvement from previous Cmax: " 
                  << improvement * 100 << "%" << std::endl;
        resultFile << "Improvement from previous Cmax: " 
                   << improvement * 100 << "%" << std::endl;

        std::cout << "Absolute change in Cmax: " << absoluteChange << std::endl;
        resultFile << "Absolute change in Cmax: " << absoluteChange << std::endl;

        // 输出每个子图(聚类)的load和城市
        for (size_t i = 0; i < clusterGraphs.size(); ++i) {
            std::cout << "Cluster " << i + 1 << ": ";
            resultFile << "Cluster " << i + 1 << ": ";

            for (auto it = clusterGraphs[i].lSommets; it != nullptr; it = it->suivant) {
                std::cout << it->valeur->v.name << " ";
                resultFile << it->valeur->v.name << " ";
            }
            std::cout << "Load: " << clusterLoads[i] << std::endl;
            resultFile << "Load: " << clusterLoads[i] << std::endl;
        }

        // 判断是否停止
        if ((improvement < improvementThreshold && absoluteChange < 1e-3) 
            || iterationCount >= MAX_ITERATIONS) {
            std::cout << "Stopping optimization: ";
            if (improvement < improvementThreshold) {
                std::cout << "Improvement below threshold.";
            } else if (absoluteChange < 1e-3) {
                std::cout << "Cmax change below minimum threshold.";
            } else if (iterationCount >= MAX_ITERATIONS) {
                std::cout << "Reached max iterations.";
            }
            std::cout << std::endl;

            resultFile << "Stopping optimization." << std::endl;
            break;
        }

        prevCmax = currentCmax;
        bestGraph = mergeClusters(clusterGraphs);

        K++;

        // 计算单次迭代耗时
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;
        std::cout << "Iteration time: " << elapsed.count() << " seconds." << std::endl;
        resultFile << "Iteration time: " << elapsed.count() << " seconds." << std::endl;
    }

    resultFile << "Final Cmax: " << prevCmax << std::endl;
    resultFile.close();
    return bestGraph;
}

int KMeansWithOptimization::calculateClusterLoad(const Graphe<double, Ville>& cluster) {
    double totalLoad = 0.0;
    // 这里简单把所有边权值的总和视为负载
    for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
        totalLoad += it->valeur->v; // 边的权值
    }
    return static_cast<int>(totalLoad);
}

std::pair<double, std::vector<int>> 
KMeansWithOptimization::balanceClustersFPTAS(const std::vector<int>& clusterLoads, 
                                             int numClusters, 
                                             double epsilon) 
{
    // 直接调用 FPTAS_with_order (位于 fptas_algorithm.hpp)
    return FPTAS_with_order(clusterLoads, numClusters, epsilon);
}

Graphe<double, Ville> 
KMeansWithOptimization::mergeClusters(const std::vector<Graphe<double, Ville>>& clusters) {
    Graphe<double, Ville> mergedGraph;
    // 简单地把每个子图的点和边全都插入到 mergedGraph 中即可
    for (const auto& cluster : clusters) {
        // 拷贝所有城市
        for (auto it = cluster.lSommets; it != nullptr; it = it->suivant) {
            mergedGraph.creeSommet(it->valeur->v);
        }
        // 拷贝所有边
        for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
            mergedGraph.creeArete(it->valeur->v, it->valeur->debut, it->valeur->fin);
        }
    }
    return mergedGraph;
}

double 
KMeansWithOptimization::calculateMaxLoad(const std::vector<std::vector<Ville>>& clusters, 
                                         const Graphe<double, Ville>& graphe)
{
    double maxLoad = 0.0;
    for (const auto& cluster : clusters) {
        // 根据城市名提取子图
        std::unordered_set<std::string> clusterCityNames;
        for (const auto& ville : cluster) {
            clusterCityNames.insert(ville.name);
        }
        Graphe<double, Ville> subgraph = graphe.extractSubgraph(clusterCityNames);

        // 计算该子图负载
        double load = static_cast<double>(calculateClusterLoad(subgraph));
        if (load > maxLoad) {
            maxLoad = load;
        }
    }
    return maxLoad;
}
