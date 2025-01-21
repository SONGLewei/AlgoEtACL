#pragma once

#include <string>
#include "../json.hpp"
#include <fstream>
#include "Graphe.h"
#include "Ville.h"

class GrapheExporter {
public:
    /**
 * Exporter la structure du graphe en fichier JSON.
 * @param graphe   : Le graphe à exporter.
 * @param filename : Le chemin du fichier JSON exporté.
 */
    static void exporter(const Graphe<double, Ville>& graphe, const std::string& filename);
};