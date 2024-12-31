#include <iostream>
#include "../includeGraphe/JSONLoader.h"
#include "../includeGraphe/KMeansWithOptimization.h"

int main() {
    JSONLoader loader;
    std::string filePath = "../13_fichiers_JSON/GrandEst.json";
    Graphe<double, Ville> graphe = loader.chargerGrapheFromJSON(filePath);

    if (graphe.nombreSommets() == 0) {
        std::cerr << "Failed to load JSON file." << std::endl;
        return 1;
    }

    KMeansWithOptimization optimizer;
    Graphe<double, Ville> optimizedGraph = optimizer.optimizeAndReturnGraph(graphe, 0.001, 0.01);

    std::cout << "Optimization completed. Final graph generated." << std::endl;

    return 0;
}
