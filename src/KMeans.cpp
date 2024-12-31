#include "../includeGraphe/KMeans.h"
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>
#include <unordered_set>

// 一个简单的欧几里得距离，基于 (latitude, longitude)
static double distanceBetween(const Ville& a, const Ville& b) {
    double dLat = a.latitude - b.latitude;
    double dLon = a.longitude - b.longitude;
    return std::sqrt(dLat * dLat + dLon * dLon);
}

std::vector<std::vector<Ville>> kMeansClustering(const std::vector<Ville>& data, int K) {
    // 如果数据为空或者 K <= 0，直接返回
    if (data.empty() || K <= 0) {
        return {};
    }
    // 如果 K 大于数据量，则修正为数据量大小
    if (K > (int)data.size()) {
        K = (int)data.size();
    }

    // 随机数引擎
    static std::mt19937 rng{std::random_device{}()};
    // 1. 随机选 K 个点作为初始中心
    std::vector<Ville> centers;
    std::uniform_int_distribution<int> dist(0, (int)data.size() - 1);
    // 随机且不重复地选择 K 个初始中心（简单做法：允许可能重复，也可以加去重逻辑）
    std::unordered_set<int> chosen;
    while ((int)centers.size() < K) {
        int idx = dist(rng);
        // 简单处理，可不去重
        centers.push_back(data[idx]);
    }

    // 开始迭代
    bool changed = true;
    std::vector<std::vector<Ville>> clusters(K);

    while (changed) {
        changed = false;
        // 清空上一次的聚类分配
        for (auto& cluster : clusters) {
            cluster.clear();
        }

        // 2. 将每个数据点分配到最近的中心
        for (const auto& point : data) {
            double minDist = std::numeric_limits<double>::max();
            int clusterIndex = 0;
            for (int i = 0; i < K; i++) {
                double distVal = distanceBetween(point, centers[i]);
                if (distVal < minDist) {
                    minDist = distVal;
                    clusterIndex = i;
                }
            }
            clusters[clusterIndex].push_back(point);
        }

        // 3. 更新每个聚类的中心
        for (int i = 0; i < K; i++) {
            if (!clusters[i].empty()) {
                double sumLat = 0.0;
                double sumLon = 0.0;
                for (const auto& p : clusters[i]) {
                    sumLat += p.latitude;
                    sumLon += p.longitude;
                }
                double newLat = sumLat / clusters[i].size();
                double newLon = sumLon / clusters[i].size();

                // 检查中心是否有变化
                if (std::fabs(newLat - centers[i].latitude) > 1e-6 ||
                    std::fabs(newLon - centers[i].longitude) > 1e-6) {
                    changed = true;
                    centers[i].latitude = newLat;
                    centers[i].longitude = newLon;
                }
            }
        }
    }

    return clusters;
}
