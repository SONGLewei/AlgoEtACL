#pragma once

#include <string>
#include <fstream>
#include "json.hpp"

#include "Graphe.h"
#include "Ville.h"

class GrapheExporter {
public:
    /**
     * @param graphe   : graph pour exporter
     * @param filename : path of export
     */
    static void exporter(const Graphe<double, Ville>& graphe, const std::string& filename) {
        nlohmann::ordered_json j;
        
        j["noeuds"] = nlohmann::ordered_json::array();
        for (auto it = graphe.lSommets; it != nullptr; it = it->suivant) {
            const Ville& ville = it->valeur->v;
            j["noeuds"].push_back({
                {"ville", ville.name},
                {"latitude", ville.latitude},
                {"longitude", ville.longitude}
            });
        }

        
        j["aretes"] = nlohmann::ordered_json::array();
        for (auto it = graphe.lAretes; it != nullptr; it = it->suivant) {
            auto arete = it->valeur;
            const Ville& villeA = arete->debut->v;
            const Ville& villeB = arete->fin->v;

            j["aretes"].push_back({
                {"de", villeA.name},
                {"vers", villeB.name}
            });
        }

        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Erreur: impossible d'ouvrir en écriture le fichier "
                      << filename << std::endl;
            return;
        }
        outFile << j.dump(2) << std::endl; 
        outFile.close();
    }
};