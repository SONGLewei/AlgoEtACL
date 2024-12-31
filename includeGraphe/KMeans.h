#pragma once

#include <vector>
#include "Ville.h"

/**
 * @brief kMeansClustering：对一组城市做 KMeans 聚类
 * @param data   包含若干 Ville 的向量
 * @param K      聚类个数
 * @return       返回一个二维向量，每个元素代表一个聚类（聚类里是一组 Ville）
 */
std::vector<std::vector<Ville>> kMeansClustering(const std::vector<Ville>& data, int K);

