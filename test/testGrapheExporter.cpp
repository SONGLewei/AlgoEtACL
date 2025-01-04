#include <iostream>
#include "../include/Graphe/BuildNewGraphe.h"

int main(){
    JSONLoader loader;
    auto graphe = loader.chargerGrapheFromJSON("../13_fichiers_JSON/GrandEst.json");
    std::cout << "=== Graph Loaded from JSON ===\n";
    std::cout << "Number of Villes: " << graphe.nombreSommets() << "\n";
    std::cout << "Number of aretes: "    << graphe.nombreAretes()  << "\n";

    std::cout << "\nListe des villes:\n";
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        auto sommet = it->valeur;
        std::cout << "- " << sommet->v.name
                  << " (lat=" << sommet->v.latitude
                  << ", lon=" << sommet->v.longitude
                  << "), deg=" << sommet->degre << "\n";
    }

    std::cout << "\nListe des aretes (distance km):\n";
    for (auto it = graphe.lAretes; it; it = it->suivant) {
        auto arete = it->valeur;
        auto s1 = arete->debut->v.name;
        auto s2 = arete->fin->v.name;
        double dist = arete->v;
        std::cout << s1 << " <--> " << s2 << " : " << dist << " km\n";
    }

    GrapheExporter::exporter(graphe,"Graphe_complet.json");
    std::cout<<"Exporter fini. ";
    return 0;
}