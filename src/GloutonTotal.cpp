#include "../include/Algo/Glouton.h"
#include "../include/Graphe/BuildNewGraphe.h"

int main(){

    std::vector<std::vector<int>> bestClusters;
    std::vector<std::pair<double, std::vector<std::string>>> bestResults;
    std::string outputPath = "D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/resultat/Graphe_res_GL.json";
    double lastCmax = std::numeric_limits<double>::max();
    double bestCmax = std::numeric_limits<double>::max();
    int bestK = 1; 

    JSONLoader loader;
    auto graphe = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/GrandEst.json");

    //Soit on a maxim 5 camions, parce que si avec envrion 20 villes, 7 est largement
    for (int k = 1; k <= 7; k++) {
        bestCmax = std::numeric_limits<double>::max();

        for (int run = 0; run < 10; run++) {
            auto clusters = KMeansPP(graphe, k);
            auto results = calculateCmaxForAllClustersGlouton(graphe, clusters);

            double currentCmax = 0.0;
            for (const auto& [cmax, path] : results) {
                currentCmax = std::max(currentCmax, cmax);
            }

            if (currentCmax < bestCmax) {
                bestCmax = currentCmax;
                bestClusters = clusters;
                bestResults = results;
            }
        }

        if (lastCmax - bestCmax < 300) {
            std::cout << "Stop searching, because opt(i-1)-opt(i)<300 \n";//opt(i): ieme camions's performance
            break;
        }

        lastCmax = bestCmax;
        bestK = k;
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

    auto newGraphe = buildNewGrapheWithPaths(graphe, bestClusters, bestResults);
    GrapheExporter::exporter(newGraphe, outputPath);

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