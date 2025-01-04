#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "../Graphe/BuildNewGraphe.h"
#include "Kmeans.h"

/**
 * Résout le problème du TSP (cycle) pour le chemin le plus court en utilisant la programmation dynamique Held-Karp.
 * @param graphe          : Le graphe global, utilisé pour trouver les distances entre les villes
 * @param clusterCities   : Les pointeurs vers toutes les villes appartenant au cluster
 * @return pair<double, vector<string>> 
 *         - first  : Cmax (la distance totale minimale du cycle)
 *         - second : Les noms des villes dans l'ordre de visite (incluant le retour au point de départ)
 */
std::pair<double, std::vector<std::string>> 
tspDynamicProgramming(const Graphe<double, Ville>& graphe,
                      const std::vector<Sommet<Ville>*>& clusterCities);

/**
 * Calcule le cycle TSP minimal (Cmax) et le chemin de visite pour chaque cluster produit par KMeans++.
 * @param graphe   : Le graphe contenant toutes les villes et leurs distances
 * @param clusters : Le résultat de KMeans++, où chaque sous-vecteur représente un ensemble de clés de villes
 * @return         : Un vecteur de taille clusters.size(), dont chaque élément est {Cmax, path} 
 *                   où path est l'ordre de visite des noms des villes (commençant et terminant à la première ville)
 */
std::vector<std::pair<double, std::vector<std::string>>> 
calculateCmaxForAllClustersPD(const Graphe<double, Ville>& graphe,
                            const std::vector<std::vector<int>>& clusters);