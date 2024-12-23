#include <iostream>
#include "Graphe.h"
#include "GraphLoader.h"
#include "Ville.h"

int main() {
  // Création d'un graphe de villes
  Graphe<Ville, double> carte;

  // Chargement des villes depuis un fichier JSON
  GraphLoader::loadFromFile("13_fichiers_JSON/Bretagne.json", carte);

  // Affichage
  std::cout << "Carte routière :" << std::endl;
  carte.printGraph();
}