#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>
#include <string>
#include <cstring>
#include <unordered_set>
#include "../includeAlgo/fptas_algorithm.hpp"
#include "../includeAlgo/graph_coloring.hpp"
#include "../includeGraphe/Ville.h"
#include "../includeGraphe/Graphe.h"
#include "../includeGraphe/JSONLoader.h"
#include "../includeGraphe/KMeans.h"

// FPTAS: 用于负载均衡
std::pair<double, std::vector<int>> balanceClustersFPTAS(const std::vector<int>& clusterLoads, int numClusters, double epsilon) {
    return FPTAS_with_order(clusterLoads, numClusters, epsilon);
}

// Graph Coloring: 解决不兼容问题
void adjustClustersWithGraphColoring(const std::vector<int>& tasks, 
                                     const std::vector<std::vector<int>>& incompatibilityMatrix, 
                                     std::vector<int>& clusterAssignments) {
    graph_coloring(tasks, incompatibilityMatrix);
}

// 提取簇的总负载（路径长度）
int calculateClusterLoad(const Graphe<double, Ville>& cluster) {
    double totalLoad = 0.0;
    for (auto it = cluster.lAretes; it != nullptr; it = it->suivant) {
        totalLoad += it->valeur->v; // 边的权重代表路径长度
    }
    return static_cast<int>(totalLoad);
}

int main() {
    // Step 1: 加载 JSON 文件并构建图
    JSONLoader loader;
    std::string filePath = "../13_fichiers_JSON/GrandEst.json"; // 替换为您的 JSON 文件路径
    Graphe<double, Ville> graphe = loader.chargerGrapheFromJSON(filePath);

    if (graphe.nombreSommets() == 0) {
        std::cerr << "加载 JSON 文件失败。" << std::endl;
        return 1;
    }

    // Step 2: 初始 K-means 聚类
    std::vector<Ville> villes;
    for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
        villes.push_back(it->valeur->v);
    }
    int K = 3; // 假设分成 3 个聚类
    auto clusters = kMeansClustering(villes, K);

    // Step 3: 提取每个簇的子图并计算负载
    std::vector<Graphe<double, Ville>> clusterGraphs;
    std::vector<int> clusterLoads;

    for (const auto& cluster : clusters) {
        // 使用 extractSubgraph 提取子图
        std::unordered_set<std::string> clusterCityNames;
        for (const auto& ville : cluster) {
            clusterCityNames.insert(ville.name);
        }

        Graphe<double, Ville> clusterGraph = graphe.extractSubgraph(clusterCityNames);
        clusterGraphs.push_back(clusterGraph);

        // 计算簇的负载
        clusterLoads.push_back(calculateClusterLoad(clusterGraph));
    }

    // Step 4: 使用 FPTAS 负载均衡
    double epsilon = 0.1; // 调整负载均衡的精度
    auto balancedResult = balanceClustersFPTAS(clusterLoads, K, epsilon);

    if (balancedResult.first < 0) {
        std::cerr << "FPTAS 负载均衡失败。" << std::endl;
        return 1;
    }
    std::cout << "FPTAS Completed, Approximate Optimal Load: " << balancedResult.first << std::endl;

    // Step 5: 如果有不兼容城市，使用 Graph Coloring 进一步优化分配
    std::vector<std::vector<int>> incompatibilityMatrix(villes.size(), std::vector<int>(villes.size(), 0));
    // 假设某些城市之间存在冲突
    // incompatibilityMatrix[cityA][cityB] = 1; 

    std::vector<int> clusterAssignments(villes.size(), -1); // 每个城市的簇分配
    adjustClustersWithGraphColoring(clusterLoads, incompatibilityMatrix, clusterAssignments);

    // 打印最终结果
    std::cout << "Groups: " << std::endl;
    for (int i = 0; i < clusters.size(); ++i) {
        std::cout << "Cluster " << i + 1 << ": ";
        for (const auto& ville : clusters[i]) {
            std::cout << ville.name << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}
