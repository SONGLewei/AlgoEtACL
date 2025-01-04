#ifndef BUILDNEWGRAPHE_H
#define BUILDNEWGRAPHE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Graphe.h"
#include "Ville.h"
#include "JSONLoader.h"
#include "GrapheExporter.h"

/**
 * 根据聚类结果和每个簇的贪心路径，在新图中仅创建必要的城市与边。
 * @param graphe   : 原图（包含所有城市以及城市间距离）
 * @param clusters : 每个簇中城市的 key 列表（如 KMeansPP 返回的结果）
 * @param results  : 对应每个簇的 (Cmax, 路径)；路径里是城市的字符串名字，顺序为贪心算法计算得到的顺序
 * @return         : 一个包含所有城市节点，但仅包含各簇路径上相邻城市的边的“新图”
 */
Graphe<double, Ville> buildNewGrapheWithPaths(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters,
    const std::vector<std::pair<double, std::vector<std::string>>>& results);

#endif