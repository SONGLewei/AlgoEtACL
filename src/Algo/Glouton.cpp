
#include "../../include/Algo/Glouton.h"

std::pair<double, std::vector<std::string>> greedyPathForCluster(
    const Graphe<double, Ville>& graphe,
    const std::vector<int>& cluster,
    const std::unordered_map<int, Sommet<Ville>*>& keyToCity) 
{
    if (cluster.empty()) {
        return {0.0, {}};
    }

    std::vector<int> unvisited(cluster);
    std::vector<std::string> path;
    double totalDistance = 0.0;

    //Choisir un depart random
    int currentCityKey = unvisited.front();
    path.push_back(keyToCity.at(currentCityKey)->v.name);
    unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), currentCityKey), unvisited.end());

    //Glouton: Choisir la plus proche
    while (!unvisited.empty()) {
        double minDistance = std::numeric_limits<double>::max();
        int nextCityKey = -1;

        for (int candidateKey : unvisited) {
            double distance = distanceCityToCity(graphe, keyToCity.at(currentCityKey), keyToCity.at(candidateKey));
            if (distance < minDistance) {
                minDistance = distance;
                nextCityKey = candidateKey;
            }
        }

        if (nextCityKey != -1) {
            path.push_back(keyToCity.at(nextCityKey)->v.name);
            totalDistance += minDistance;
            currentCityKey = nextCityKey;
            unvisited.erase(std::remove(unvisited.begin(), unvisited.end(), nextCityKey), unvisited.end());
        }
    }

    //Pour avoir une cycle
    if (!path.empty()) {
        double returnDistance = distanceCityToCity(graphe, keyToCity.at(cluster.front()), keyToCity.at(currentCityKey));
        totalDistance += returnDistance;
        path.push_back(keyToCity.at(cluster.front())->v.name);
    }

    return {totalDistance, path};
}

std::vector<std::pair<double, std::vector<std::string>>> calculateCmaxForAllClustersGlouton(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters)
{
    //a partir de clef a ville
    std::unordered_map<int, Sommet<Ville>*> keyToCity;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        keyToCity[it->valeur->clef] = it->valeur;
    }

    //Stock le Cmax et path pour chaqu'un
    std::vector<std::pair<double, std::vector<std::string>>> results;

    for (const auto& cluster : clusters) {
        auto result = greedyPathForCluster(graphe, cluster, keyToCity);
        results.push_back(result);
    }

    return results;
}