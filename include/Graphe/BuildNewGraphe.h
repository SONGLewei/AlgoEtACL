#ifndef BUILDNEWGRAPHE_H
#define BUILDNEWGRAPHE_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Graphe.h"
#include "Ville.h"
#include "JSONLoader.h"
#include "GrapheExporter.h"

/**
 * Crée un nouveau graphe contenant uniquement les villes et les arêtes nécessaires, 
 * basé sur les résultats de clustering et les chemins de chaque cluster.
 * @param graphe   : Le graphe original (contenant toutes les villes et les distances entre elles).
 * @param clusters : Liste des clés des villes dans chaque cluster (par exemple, le résultat de KMeansPP).
 * @param results  : Pour chaque cluster, une paire (Cmax, chemin), où le chemin est une liste de noms de villes 
 *                   dans l'ordre calculé par les algorithme.
 * @return         : Un "nouveau graphe" contenant tous les nœuds de ville mais seulement les arêtes 
 *                   entre les villes adjacentes dans les chemins des clusters.
 */
Graphe<double, Ville> buildNewGrapheWithPaths(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters,
    const std::vector<std::pair<double, std::vector<std::string>>>& results);

#endif