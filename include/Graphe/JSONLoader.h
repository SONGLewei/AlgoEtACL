#pragma once

#include "Graphe.h"
#include "Ville.h"
#include "../json.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <vector>

using json = nlohmann::json;

class JSONLoader {
public:
    /**
 * Charger les données du graphe à partir d'un fichier JSON.
 * @param fichier : Le chemin du fichier JSON.
 * @return        : Renvoie la structure du graphe chargée.
 */
    Graphe<double, Ville> chargerGrapheFromJSON(const std::string& fichier);

    /**
 * Calculer la distance de Haversine entre deux points.
 * @param lat1 : Latitude du premier point.
 * @param lon1 : Longitude du premier point.
 * @param lat2 : Latitude du deuxième point.
 * @param lon2 : Longitude du deuxième point.
 * @return     : Renvoie la distance entre les deux points (en kilomètres).
 */
    double haversineDistance(double lat1, double lon1, double lat2, double lon2);
};