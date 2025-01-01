#pragma once

#include <vector>
#include <unordered_set>
#include "Ville.h"
#include "Graphe.h"

/**
 * @brief KMeansWithOptimization 类:
 * - 对图进行 KMeans 聚类，并通过最小生成树 (MST) 合并子图。
 */
class KMeansWithOptimization {
public:
    /**
     * @brief 优化图的主函数
     * @param graphe 原始图
     * @param epsilon FPTAS 的精度
     * @param improvementThreshold 停止优化的最小改进阈值
     * @return 优化后的图
     */
    Graphe<double, Ville> optimizeAndReturnGraph(
        const Graphe<double, Ville>& graphe,
        double epsilon,
        double improvementThreshold
    );

    /**
     * @brief 计算一个子图的最小生成树
     * @param cluster 输入的子图
     * @return 子图的最小生成树
     */
    Graphe<double, Ville> generateMST(const Graphe<double, Ville>& cluster);

    /**
     * @brief 合并所有聚类的子图
     * @param clusters 子图列表
     * @return 合并后的图
     */
    Graphe<double, Ville> mergeClusters(const std::vector<Graphe<double, Ville>>& clusters);

private:
    /**
     * @brief 计算子图的负载
     * @param cluster 子图
     * @return 子图的负载
     */
    int calculateClusterLoad(const Graphe<double, Ville>& cluster);

    /**
     * @brief 使用 FPTAS 平衡负载
     * @param clusterLoads 每个子图的负载
     * @param numClusters 子图数量
     * @param epsilon FPTAS 精度
     * @return 平衡后的 (Cmax, 分配方案)
     */
    std::pair<double, std::vector<int>> balanceClustersFPTAS(
        const std::vector<int>& clusterLoads,
        int numClusters,
        double epsilon
    );

    /**
     * @brief 并行计算所有子图的负载
     * @param graphs 子图列表
     * @return 负载列表
     */
    std::vector<int> parallelCalculateClusterLoads(const std::vector<Graphe<double, Ville>>& graphs);
};

