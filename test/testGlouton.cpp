#include "../include/Algo/Glouton.h"
#include "../include/Graphe/BuildNewGraphe.h"
#include <unordered_map>
#include <vector>
#include <string>


int main() {
    // 1) 加载原图
    JSONLoader loader;
    auto graphe = loader.chargerGrapheFromJSON("../13_fichiers_JSON/GrandEst.json");
    std::cout << "=== Graph Loaded from JSON ===\n";
    //std::cout << "Number of Villes: " << graphe.nombreSommets() << "\n";
    //std::cout << "Number of aretes: " << graphe.nombreAretes()  << "\n\n";

    // 2) 使用KMeans++进行聚类
    int k = 2; // 可自行调整想要的簇数量
    auto clusters = KMeansPP(graphe, k);

    // 3) 针对每个簇，计算其贪心路径(及 Cmax)
    auto results = calculateCmaxForAllClustersGlouton(graphe, clusters);

    // 4) 打印结果
    std::cout << "=== Cmax 和 路径 ===\n";
    for (size_t i = 0; i < results.size(); i++) {
        const auto& [cmax, path] = results[i];
        std::cout << "簇 " << i << ":\n";
        std::cout << "  Cmax: " << cmax << " km\n";
        std::cout << "  路径: ";
        for (const auto& cityName : path) {
            std::cout << cityName << " ";
        }
        std::cout << "\n\n";
    }

    // 5) 基于这些簇的贪心路径，构建新图
    auto newGraphe = buildNewGrapheWithPaths(graphe, clusters, results);

    // 6) 导出新图（JSON 格式）
    GrapheExporter::exporter(newGraphe, "Graphe_res.json");

    return 0;
}