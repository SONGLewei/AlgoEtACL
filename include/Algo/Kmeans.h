#pragma once
#include <vector>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <ctime>
#include <algorithm>
#include "../Graphe/BuildNewGraphe.h"

/**
 * Pour retourner tous les villes
 * @param cities: tous les ville
 */
std::vector<Sommet<Ville>*> getAllCities(const Graphe<double, Ville>& graphe);

/**
 * Pour calculer la distance entre une ville et la centre en utilisent la formule haversine
 */
double distanceCityToCentroid(const Sommet<Ville>* city, double latC, double lonC);

/**
 * Pour retourner la distance entre les 2 villes
 */
double distanceCityToCity(const Graphe<double, Ville>& graphe,
                          const Sommet<Ville>* c1,
                          const Sommet<Ville>* c2);

/**
 * K-means++ inital: choisir k villes initial comme la centre de masse(质心)
 * La premiere choisir aleatoire, la suivante en utilisent D(x)^2
 * @param centers: les sous marques des villes
 *  Pointer sur un node d'un ville de cities vector
 */
std::vector<int> kmeansPlusPlusInit(const Graphe<double, Ville>& graphe,
                                    const std::vector<Sommet<Ville>*>& cities,
                                    int k);

/**
 * La fonction main ici, pour realiser le Kmeans++
 * retourner le clef de chaque ville
 * @param graphe : le graphe contenant toutes les villes et distances
 * @param k : le nombre de clusters
 */
std::vector<std::vector<int>> KMeansPP(const Graphe<double, Ville>& graphe, int k);