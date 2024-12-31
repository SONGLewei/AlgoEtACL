#pragma once

#include <vector>
#include <utility>       // for std::pair
#include <unordered_set> // for std::unordered_set
#include "Graphe.h"
#include "Ville.h"

/**
 * KMeansWithOptimization 类：
 *  - 用于对给定的地理位置（Ville）的图做KMeans聚类，然后再用FPTAS做负载平衡，示例性优化流程。
 */
class KMeansWithOptimization {
public:
    /**
     * @brief 对图进行KMeans+FPTAS综合优化
     * @param graphe               原始图
     * @param epsilon              FPTAS算法的精度参数
     * @param improvementThreshold 停止优化的最小改进阈值
     * @return 优化后(或迭代到最后)得到的合并图
     */
    Graphe<double, Ville> optimizeAndReturnGraph(
        const Graphe<double, Ville>& graphe, 
        double epsilon, 
        double improvementThreshold
    );

private:
    /**
     * @brief 计算一个子图(通常是聚类后)的负载（这里简单示例为所有边权值之和）
     * @param cluster 子图
     * @return 该子图的负载(整型)
     */
    int calculateClusterLoad(const Graphe<double, Ville>& cluster);

    /**
     * @brief 使用FPTAS来平衡所有聚类负载
     * @param clusterLoads  每个聚类的负载列表
     * @param numClusters   聚类个数
     * @param epsilon       FPTAS精度
     * @return (Cmax, 分配情况)。其中Cmax是近似最优的最大负载。
     */
    std::pair<double, std::vector<int>> balanceClustersFPTAS(
        const std::vector<int>& clusterLoads, 
        int numClusters, 
        double epsilon
    );

    /**
     * @brief 将若干子图(对应不同聚类)合并为一个图
     * @param clusters 聚类得到的若干子图
     * @return 合并后的图
     */
    Graphe<double, Ville> mergeClusters(
        const std::vector<Graphe<double, Ville>>& clusters
    );

    /**
     * @brief 计算当前所有聚类中的最大负载
     * @param clusters  若干个聚类，每个聚类是一组 Ville
     * @param graphe    原始图，用来提取子图并计算其负载
     * @return 最大负载(双精度)
     */
    double calculateMaxLoad(
        const std::vector<std::vector<Ville>>& clusters, 
        const Graphe<double, Ville>& graphe
    );
};

