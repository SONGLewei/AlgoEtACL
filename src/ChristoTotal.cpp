#include "../include/Algo/Christo.h"
#include "../include/Graphe/BuildNewGraphe.h"

int main(){

    std::vector<std::vector<int>> bestClusters;
    std::vector<std::pair<double, std::vector<std::string>>>  bestResults;
    std::string outputPath = "D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/resultat/Graphe_res_CH.json";
    double lastCmax = std::numeric_limits<double>::max(); // 上一次的Cmax
    double bestCmax = std::numeric_limits<double>::max(); // 当前k下最优Cmax
    int bestK = 1; 

    JSONLoader loader;
    //auto graphe = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/GrandEst.json");
    auto graphe = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Auvergne-Rhone-Alpes.json");
    //auto grapheBO = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Bourgogne-Franche-Comte.json");
    //auto grapheBR = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Bretagne.json");
    //auto grapheCE = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Centre-Val_de_Loire.json");
    //auto grapheCO = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Corse.json");
    //auto grapheHA = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/HautDeFrance.json");
    //auto grapheIL = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Ile-de-France.json");
    //auto grapheNO = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Normandie.json");
    //auto grapheNU = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/NouvelleAquitaine.json");
    //auto grapheOC = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Occitanie.json");
    //auto graphePA = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Pays_de_la_Loire.json");
    //auto graphePR = loader.chargerGrapheFromJSON("D:/M1/Projet ALOG ACL/ProjetPartie3/Projet_Algo/13_fichiers_JSON/Provence_Alpes_Cote_Azur.json");


    for (int k = 1; k <= 7; k++) {
        bestCmax = std::numeric_limits<double>::max();

        for (int run = 0; run < 3; run++) {
            auto clusters = KMeansPP(graphe, k);
            auto results = calculateCmaxForAllClustersCL(graphe, clusters);

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
            std::cout << "Stop searching, because CMax < 300. \n";
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

        for (size_t j = 0; j < path.size(); ++j) {
            std::cout << path[j];
            if (j != path.size() - 1) {
                std::cout << " -> ";
            }
        }
        std::cout << "\n\n";
    }

    auto newGraphe = buildNewGrapheWithPaths(graphe, bestClusters, bestResults);
    GrapheExporter::exporter(newGraphe, outputPath);
    
    return 0;
}