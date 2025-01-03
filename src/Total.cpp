#include "../includeAlgo/Glouton.h"
#include "../includeGraphe/BuildNewGraphe.h"

int main(){

    std::vector<std::vector<int>> bestClusters;
    std::vector<std::pair<double, std::vector<std::string>>> bestResults;
    double lastCmax = std::numeric_limits<double>::max(); // 上一次的Cmax
    double bestCmax = std::numeric_limits<double>::max(); // 当前k下最优Cmax
    int bestK = 1; 
    // 1) 加载原图
    JSONLoader loader;
    auto graphe = loader.chargerGrapheFromJSON("../13_fichiers_JSON/GrandEst.json");

    // 从 k=1 到 k=5 遍历
    for (int k = 1; k <= 5; k++) {
        bestCmax = std::numeric_limits<double>::max(); // 每个k开始时初始化最优Cmax

        // 运行5次KMeansPP并取Cmax最小的结果
        for (int run = 0; run < 10; run++) {
            auto clusters = KMeansPP(graphe, k);
            auto results = calculateCmaxForAllClusters(graphe, clusters);

            // 计算当前的最大Cmax（取每个簇的Cmax最大值）
            double currentCmax = 0.0;
            for (const auto& [cmax, path] : results) {
                currentCmax = std::max(currentCmax, cmax);
            }

            // 如果当前Cmax更小，则更新最优结果
            if (currentCmax < bestCmax) {
                bestCmax = currentCmax;
                bestClusters = clusters;
                bestResults = results;
            }
        }

        // 比较Cmax减幅
        if (lastCmax - bestCmax < 300) {
            std::cout << "停止搜索，因为Cmax减幅小于300。\n";
            break;
        }

        // 更新lastCmax为当前最优Cmax
        lastCmax = bestCmax;
        bestK = k; // 保存当前的k值
    }

    std::cout << "=== Cmax et Path ===\n";
    for (size_t i = 0; i < bestResults.size(); i++) {
        const auto& [cmax, path] = bestResults[i];
        std::cout << "Camion " << i << ":\n";
        std::cout << "  Cmax: " << cmax << " km\n";
        std::cout << "  Path: ";
        for (const auto& cityName : path) {
            std::cout << cityName <<" -> " << " ";
        }
        std::cout << "\n\n";
    }

    // 5) 基于这些簇的贪心路径，构建新图
    auto newGraphe = buildNewGrapheWithPaths(graphe, bestClusters, bestResults);
    GrapheExporter::exporter(newGraphe, "Graphe_res.json");
    

    return 0;
}

/*
怎么打印所有路径
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
    }*/