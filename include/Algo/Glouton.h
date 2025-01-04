#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../Graphe/BuildNewGraphe.h"
#include "Kmeans.h"

/**
 * Calcule le chemin glouton pour un cluster.
 * @param graphe     : La structure du graphe contenant les villes et les distances
 * @param cluster    : Les clés des villes appartenant au cluster actuel
 * @param keyToCity  : Une correspondance fournissant la relation entre les clés et les informations des villes
 * @return Renvoie la longueur du chemin pour le cluster actuel (Cmax) et l'ordre des visites (noms des villes)
 */
std::pair<double, std::vector<std::string>> greedyPathForCluster(
    const Graphe<double, Ville>& graphe,
    const std::vector<int>& cluster,
    const std::unordered_map<int, Sommet<Ville>*>& keyToCity);

/**
 * Calcule les chemins gloutons pour tous les clusters.
 * @param graphe   : La structure du graphe contenant les villes et les distances
 * @param clusters : Un ensemble de clés représentant les villes de chaque cluster
 * @return Renvoie le Cmax et le chemin (noms des villes) pour chaque cluster
 */
std::vector<std::pair<double, std::vector<std::string>>> calculateCmaxForAllClustersGlouton(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters);