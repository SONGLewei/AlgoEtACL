#include <iostream>
#include <string>

// 如果你的文件结构是：项目/includeGraphe/JSONLoader.h
// 那就改为 #include "../includeGraphe/JSONLoader.h"
// 同理，KMeansWithOptimization.h 也可根据路径调整
#include "../includeGraphe/JSONLoader.h"
#include "../includeGraphe/KMeansWithOptimization.h"
#include "../includeGraphe/Graphe.h"
#include "../includeGraphe/Ville.h"

int main() {
    // 1. 加载 JSON 文件，构造图
    JSONLoader loader;
    std::string filePath = "../13_fichiers_JSON/GrandEst.json";  
    // 请根据实际项目中文件路径修改

    Graphe<double, Ville> graphe = loader.chargerGrapheFromJSON(filePath);
    if (graphe.nombreSommets() == 0) {
        std::cerr << "Failed to load JSON file or the graph is empty." << std::endl;
        return 1;
    }

    // 2. 打印一下加载后的图信息（可选）
    std::cout << "Loaded Graph:\n";
    std::cout << graphe.toString() << std::endl;

    // 3. 使用KMeansWithOptimization进行聚类 + FPTAS优化
    double epsilon = 0.001;           // FPTAS的精度
    double improvementThreshold = 0.01; // 当改进小于1%时，停止迭代
    KMeansWithOptimization optimizer;

    // 调用优化函数
    Graphe<double, Ville> optimizedGraph = 
        optimizer.optimizeAndReturnGraph(graphe, epsilon, improvementThreshold);

    // 4. 打印优化后的图信息
    std::cout << "\nOptimization completed.\nFinal (merged) graph generated:\n";
    std::cout << optimizedGraph.toString() << std::endl;

    return 0;
}
