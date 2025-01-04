#pragma once

#include "Graphe.h"
#include "Ville.h"
#include "../json.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

using json = nlohmann::json;

class JSONLoader {
public:
    /**
     * 从 JSON 文件加载图数据。
     * @param fichier : JSON 文件路径
     * @return 返回加载的图结构
     */
    Graphe<double, Ville> chargerGrapheFromJSON(const std::string& fichier);

    /**
     * 计算两点之间的 Haversine 距离。
     * @param lat1 : 第一个点的纬度
     * @param lon1 : 第一个点的经度
     * @param lat2 : 第二个点的纬度
     * @param lon2 : 第二个点的经度
     * @return 返回两点之间的距离（单位：公里）
     */
    double haversineDistance(double lat1, double lon1, double lat2, double lon2);
};