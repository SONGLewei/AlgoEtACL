#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeGraphe/KMeans.h"
#include "../includeAlgo/fptas_algorithm.hpp"
#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include <cmath>
#include <unordered_set>
#include <algorithm>
#include <thread>
#include <future>

double calculateEpsilon(const std::vector<int>& clusterLoads) {
    double maxLoad = *std::max_element(clusterLoads.begin(), clusterLoads.end());
    return std::max(0.01, std::min(0.1, (double)clusterLoads.size() / maxLoad));
}

std::vector<int> KMeansWithOptimization::parallelCalculateClusterLoads(const std::vector<Graphe<double, Ville>>& graphs) {
    std::vector<int> clusterLoads(graphs.size());
    std::vector<std::future<int>> futures;
    for (size_t i = 0; i < graphs.size(); ++i) {
        futures.push_back(std::async(std::launch::async, [this, &graphs, i]() {
            return this->calculateClusterLoad(graphs[i]);
        }));
    }
    for (size_t i = 0; i < futures.size(); ++i) {
        clusterLoads[i] = futures[i].get();
    }
    return clusterLoads;
}

Graphe<double, Ville> KMeansWithOptimization::optimizeAndReturnGraph(
    const Graphe<double, Ville>& graphe,
    double epsilon,
    double improvementThreshold
) {
    int K = 1;
    double prevCmax = std::numeric_limits<double>::max();
    Graphe<double, Ville> bestGraph;
    const int MAX_ITERATIONS = 100;
    const int MIN_ITERATIONS = 5;
    int iterationCount = 0;

    std::vector<std::vector<Ville>> bestClusters;

    double initialCmax = calculateClusterLoad(graphe);
    std::cout << "Initial Cmax: " << initialCmax << "\n";

    while (true) {
        auto start = std::chrono::high_resolution_clock::now();

        // 收集所有节点
        std::vector<Ville> villes;
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            villes.push_back(it->valeur->v);
        }

        // KMeans 聚类
        auto currentClusters = kMeansClustering(villes, K);

        // 打印当前聚类结果
        std::cout << "K = " << K << ", Number of Clusters: " << currentClusters.size() << "\n";

        // 提取子图
        std::vector<Graphe<double, Ville>> clusterGraphs;
        clusterGraphs.reserve(K);
        for (const auto& cluster : currentClusters) {
            std::unordered_set<std::string> cityNames;
            for (const auto& ville : cluster) cityNames.insert(ville.name);
            Graphe<double, Ville> subgraph = graphe.extractSubgraph(cityNames);

            // 检查子图是否有效
            if (subgraph.nombreSommets() == 0) {
                std::cerr << "Warning: Extracted subgraph has no nodes. Skipping this cluster.\n";
                continue;
            }

            std::cout << "Subgraph node count: " << subgraph.nombreSommets()
                      << ", edge count: " << subgraph.nombreAretes() << "\n";

            clusterGraphs.push_back(subgraph);
        }

        // 如果 clusterGraphs 为空，打印调试信息并跳过当前迭代
        if (clusterGraphs.empty()) {
            std::cerr << "Error: All extracted subgraphs are empty. Skipping this iteration.\n";
            K++;
            iterationCount++;
            continue;
        }

        // 并行计算负载
        auto clusterLoads = parallelCalculateClusterLoads(clusterGraphs);

        // 清理 clusterGraphs，释放内存
        clusterGraphs.clear();
        clusterGraphs.shrink_to_fit();

        // 调整 epsilon
        epsilon = std::max(0.01, calculateEpsilon(clusterLoads));

        // 平衡负载
        auto balancedResult = balanceClustersFPTAS(clusterLoads, K, epsilon);
        double currentCmax = balancedResult.first;

        // 打印负载分布
        std::cout << "Load Distribution: ";
        for (const auto& load : clusterLoads) {
            std::cout << load << " ";
        }
        std::cout << "\n";

        // 打印当前迭代的 Cmax
        std::cout << "Iteration " << iterationCount + 1 << " - K = " << K 
                  << ", Current Approx Cmax = " << currentCmax << "\n";

        if (currentCmax < prevCmax) {
            bestGraph = mergeClusters(clusterGraphs);
            bestClusters = currentClusters;
            prevCmax = currentCmax;
        }

        // 判断停止条件
        if ((iterationCount >= MIN_ITERATIONS && prevCmax - currentCmax < improvementThreshold) || 
            K >= villes.size() || 
            iterationCount >= MAX_ITERATIONS) {
            if (iterationCount >= MAX_ITERATIONS) {
                std::cout << "Stopped due to reaching maximum iterations.\n";
            } else if (K >= villes.size()) {
                std::cout << "Stopped because K reached the maximum number of nodes.\n";
            } else {
                std::cout << "Stopped because improvement is below threshold.\n";
            }
            break;
        }

        K += 1;
        iterationCount++;
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "Iteration time: " 
                  << std::chrono::duration<double>(end - start).count() << " seconds.\n";
    }

    // 验证 bestGraph 的有效性
    std::cout << "Best graph node count: " << bestGraph.nombreSommets() << "\n";
    std::cout << "Best graph edge count: " << bestGraph.nombreAretes() << "\n";

    // 如果 bestGraph 为空，打印警告信息
    if (bestGraph.nombreSommets() == 0) {
        std::cerr << "Warning: bestGraph has no nodes. Returning empty graph.\n";
        return Graphe<double, Ville>(); // 返回一个空图，避免程序崩溃
    }

    return bestGraph;
}

int KMeansWithOptimization::calculateClusterLoad(const Graphe<double, Ville>& cluster) {
    double totalLoad = 0.0;
    for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
        totalLoad += it->valeur->v; 
    }
    return static_cast<int>(totalLoad);
}

std::pair<double, std::vector<int>> KMeansWithOptimization::balanceClustersFPTAS(
    const std::vector<int>& clusterLoads, 
    int numClusters, 
    double epsilon
) {
    return FPTAS_with_order(clusterLoads, numClusters, epsilon);
}

Graphe<double, Ville> KMeansWithOptimization::mergeClusters(const std::vector<Graphe<double, Ville>>& clusters) {
    Graphe<double, Ville> mergedGraph;

    if (clusters.empty()) {
        std::cerr << "Error: No clusters to merge.\n";
        return mergedGraph; // 返回空图，避免崩溃
    }

    std::cout << "Merging " << clusters.size() << " clusters.\n";
    for (size_t i = 0; i < clusters.size(); ++i) {
        std::cout << "Cluster " << i + 1 << " node count: " << clusters[i].nombreSommets() 
                  << ", edge count: " << clusters[i].nombreAretes() << "\n";
    }

    for (const auto& cluster : clusters) {
        for (auto it = cluster.lSommets; it != nullptr; it = it->suivant) {
            mergedGraph.creeSommet(it->valeur->v);
        }
        for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
            mergedGraph.creeArete(it->valeur->v, it->valeur->debut, it->valeur->fin);
        }
    }

    std::cout << "Merged graph node count: " << mergedGraph.nombreSommets()
              << ", edge count: " << mergedGraph.nombreAretes() << "\n";
    return mergedGraph;
}
