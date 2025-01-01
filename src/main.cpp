#include <iostream>
#include <string>
#include "../includeGraphe/JSONLoader.h"
#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeGraphe/Graphe.h"
#include "../includeGraphe/Ville.h"

int main() {
    try {
        // 1. 加载 JSON 文件，构造图
        JSONLoader loader;
        std::string filePath = "../13_fichiers_JSON/GrandEst.json";  

        Graphe<double, Ville> graphe = loader.chargerGrapheFromJSON(filePath);
        if (graphe.nombreSommets() == 0) {
            std::cerr << "Failed to load JSON file or the graph is empty." << std::endl;
            return 1;
        }

        std::cout << "Graph loaded successfully. Number of nodes: " 
                  << graphe.nombreSommets() << "\n";

        // 2. 使用KMeansWithOptimization进行聚类 + FPTAS优化
        double epsilon = 0.01;              // FPTAS的精度
        double improvementThreshold = 0.01; // 当改进小于1%时，停止迭代
        KMeansWithOptimization optimizer;

        std::cout << "Starting optimization process...\n";

        // 调用优化函数
        Graphe<double, Ville> optimizedGraph = 
            optimizer.optimizeAndReturnGraph(graphe, epsilon, improvementThreshold);

        // 检查结果是否有效
        if (optimizedGraph.nombreSommets() == 0) {
            std::cerr << "Optimization failed to produce a valid graph." << std::endl;
            return 1;
        }

        std::cout << "Optimization completed successfully.\n";

    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
