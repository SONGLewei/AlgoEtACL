#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <unordered_map>
#include "../includeGraphe/GrapheAll.h"
#include "../includeAlgo/Kmeans.h"

/**
 * 找到一个贪心算法的路径
 * @param graphe : 图结构，包含城市和距离
 * @param cluster : 当前簇内城市的clef
 * @param keyToCity : 一个映射，提供clef到城市信息的映射
 * @return 返回当前簇的路径长度（Cmax）和访问路径顺序（城市名字）
 */
std::pair<double, std::vector<std::string>> greedyPathForCluster(
    const Graphe<double, Ville>& graphe,
    const std::vector<int>& cluster,
    const std::unordered_map<int, Sommet<Ville>*>& keyToCity) 
{
    if (cluster.empty()) {
        return {0.0, {}}; // 如果簇为空，则返回空路径和0的Cmax
    }

    // 1) 初始化
    std::vector<int> unvisited(cluster); // 尚未访问的城市列表
    std::vector<std::string> path;       // 存储访问顺序（城市名字）
    double totalDistance = 0.0;          // 总距离

    // 随机选择一个起点，或者使用簇内的第一个城市作为起点
    int currentCityKey = unvisited.front();
    path.push_back(keyToCity.at(currentCityKey)->v.name); // 添加城市名字到路径
    unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), currentCityKey), unvisited.end());

    // 2) 贪心算法：每次选择距离最近的未访问城市
    while (!unvisited.empty()) {
        double minDistance = std::numeric_limits<double>::max();
        int nextCityKey = -1;

        for (int candidateKey : unvisited) {
            double distance = distanceCityToCity(graphe, keyToCity.at(currentCityKey), keyToCity.at(candidateKey));
            if (distance < minDistance) {
                minDistance = distance;
                nextCityKey = candidateKey;
            }
        }

        // 更新路径和状态
        if (nextCityKey != -1) {
            path.push_back(keyToCity.at(nextCityKey)->v.name); // 添加城市名字到路径
            totalDistance += minDistance;
            currentCityKey = nextCityKey;
            unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), nextCityKey), unvisited.end());
        }
    }

    // 3) 如果需要形成一个环路，可以加回起点的距离（可选）
    if (!path.empty()) {
        double returnDistance = distanceCityToCity(graphe, keyToCity.at(cluster.front()), keyToCity.at(currentCityKey));
        totalDistance += returnDistance;
        path.push_back(keyToCity.at(cluster.front())->v.name); // 添加起点城市名字形成环路
    }

    return {totalDistance, path};
}

/**
 * 为所有簇计算贪心路径，并存储每个簇的Cmax
 * @param graphe : 图结构，包含城市和距离
 * @param clusters : 每个簇的城市clef集合
 * @return 返回每个簇的Cmax和路径（城市名字）
 */
std::vector<std::pair<double, std::vector<std::string>>> calculateCmaxForAllClusters(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters)
{
    // 映射clef到城市信息
    std::unordered_map<int, Sommet<Ville>*> keyToCity;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        keyToCity[it->valeur->clef] = it->valeur;
    }

    // 存储每个簇的Cmax和路径
    std::vector<std::pair<double, std::vector<std::string>>> results;

    for (const auto& cluster : clusters) {
        auto result = greedyPathForCluster(graphe, cluster, keyToCity);
        results.push_back(result);
    }

    return results;
}