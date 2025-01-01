#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <limits>
#include <ctime>
#include <algorithm>

#include "../includeGraphe/GrapheAll.h"

/**
 * Pour retourner tous les villes
 * @param cities: tous les ville
 */
std::vector<Sommet<Ville>*> getAllCities(const Graphe<double, Ville>& graphe) {
    std::vector<Sommet<Ville>*> cities;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        cities.push_back(it->valeur);
    }
    return cities;
}

/**
 * Pour calculer la distance entre une ville et la centre en utilisent la formule haversine
 */
double distanceCityToCentroid(const Sommet<Ville>* city, double latC, double lonC) {
    static const double R = 6371.0;
    auto toRad = [](double deg) { return deg * 3.14159 / 180.0; };

    double lat1 = city->v.latitude;
    double lon1 = city->v.longitude;
    
    double dLat = toRad(latC - lat1);
    double dLon = toRad(lonC - lon1);

    double a = std::sin(dLat/2) * std::sin(dLat/2)
             + std::cos(toRad(lat1)) * std::cos(toRad(latC))
             * std::sin(dLon/2) * std::sin(dLon/2);

    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    double dist = R * c;
    return dist;
}

/**
 * Pour retourner la distance entre les 2 villes
 */
double distanceCityToCity(const Graphe<double, Ville>& graphe,
                          const Sommet<Ville>* c1,
                          const Sommet<Ville>* c2)
{
    Arete<double, Ville>* arete = graphe.getAreteParSommets(c1, c2);
        return arete->v;
}

/**
 * K-means++ inital: choisir k villes initial comme la centre de masse(质心)
 * La premiere choisir aleatoire, la suivante en utilisent D(x)^2
 * @param centers: les sous marques des villes
 *  Pointer sur un node d'un ville de cities vector
 */
std::vector<int> kmeansPlusPlusInit(const Graphe<double, Ville>& graphe,
                                    const std::vector<Sommet<Ville>*>& cities,
                                    int k)
{
    std::vector<int> centers;
    centers.reserve(k);

    static std::mt19937 gen( static_cast<unsigned int>(std::time(nullptr)) );

    //la premiere
    std::uniform_int_distribution<> distIndex(0, (int)cities.size()-1);
    int firstCenterIndex = distIndex(gen);
    centers.push_back(firstCenterIndex);

    /*La suite, D(x)= jusqu les centre la plus proche (到已选中心最近的距离)
      la probabilite: D(x)^2*/
    for (int c = 1; c < k; c++) {
        std::vector<double> minDistances(cities.size(), 0.0);

        for (int i = 0; i < (int)cities.size(); i++) {
            double dMin = std::numeric_limits<double>::max();
            for (auto centerIndex : centers) {
                double d = distanceCityToCity(graphe, cities[i], cities[centerIndex]);
                if (d < dMin) {
                    dMin = d;
                }
            }
            minDistances[i] = dMin;
        }

        double sumD2 = 0.0;
        for (double d : minDistances) {
            sumD2 += (d * d);
        }

        std::uniform_real_distribution<> distReal(0.0, sumD2);
        double r = distReal(gen);

        double cumulative = 0.0;
        int chosenIndex = -1;
        for (int i = 0; i<(int)cities.size(); i++) {
            cumulative += (minDistances[i] * minDistances[i]);
            if (cumulative >= r) {
                chosenIndex = i;
                break;
            }
        }
        centers.push_back(chosenIndex);
    }

    return centers;
}

/**
 * La fonction main ici, pour realiser le Kmeans++
 * retourner le clef de chaque ville
 * @param cities : tous les villes
 * @param centerIndices : les centre de chaque cluster ( ce sont les villes)
 */
std::vector<std::vector<int>> KMeansPP(const Graphe<double, Ville>& graphe, int k)
{
    //0) Collecter la liste des villes
    std::vector<Sommet<Ville>*> cities = getAllCities(graphe);
    int n = (int)cities.size();

    //1) Utilisez K-Means++ pour sélectionner k "centres initiaux" (ce sont de véritables villes)
    //1) 用 K-Means++ 选取 k 个“初始中心” (它们是实际的城市)
    std::vector<int> centerIndices = kmeansPlusPlusInit(graphe, cities, k);

    //2) Rendre le "centre" actualisable de manière itérative (lat, lon)
    //Étant donné que K-Means doit mettre à jour le "centroïde" à l'avenir, le centroïde peut ne pas être la ville d'origine.
    //2) 把“中心”也做成可迭代更新的 (lat, lon)
    //因为后续 K-Means 要做“质心”更新, 质心可能不是原城市
    std::vector<double> centerLats(k, 0.0);
    std::vector<double> centerLons(k, 0.0);

    for (int i = 0; i < k; i++) {
        centerLats[i] = cities[centerIndices[i]]->v.latitude;
        centerLons[i] = cities[centerIndices[i]]->v.longitude;
    }

    bool changed = true;
    int maxIterations = 100;
    std::vector<int> assignments(n, -1);

    while (changed && maxIterations-- > 0) {
        changed = false;

        //a) assignment step: Attribuez à chaque ville le centroïde le plus proche
        for (int i = 0; i < n; i++) {
            double dMin = std::numeric_limits<double>::max();
            int bestCluster = -1;
            for (int c = 0; c < k; c++) {
                double d = distanceCityToCentroid(cities[i], centerLats[c], centerLons[c]);
                if (d < dMin) {
                    dMin = d;
                    bestCluster = c;
                }
            }
            if (assignments[i] != bestCluster) {
                assignments[i] = bestCluster;
                changed = true;
            }
        }

        //b) étape de mise à jour : Recalculer le centroïde de chaque cluster (prendre la latitude et la longitude moyennes de tous les points)
        //S'il n'y a aucun intérêt dans un certain cluster (cas extrême), gardez-le inchangé ou choisissez-en un au hasard
        //b) update step: 重新计算各簇的质心(取所有点的纬度均值、经度均值)
        //如果有某个簇没有点(极端情况)，保持不变或随机挑一个
        std::vector<double> sumLat(k, 0.0);
        std::vector<double> sumLon(k, 0.0);
        std::vector<int>    count(k, 0);

        for (int i = 0; i < n; i++) {
            int c = assignments[i];
            sumLat[c] += cities[i]->v.latitude;
            sumLon[c] += cities[i]->v.longitude;
            count[c]  += 1;
        }

        for (int c = 0; c < k; c++) {
            if (count[c] > 0) {
                double newLat = sumLat[c] / count[c];
                double newLon = sumLon[c] / count[c];
                if (std::fabs(newLat - centerLats[c]) > 1e-7 ||
                    std::fabs(newLon - centerLons[c]) > 1e-7) {
                    centerLats[c] = newLat;
                    centerLons[c] = newLon;
                }
            }
        }
    }

    // return le clef
    std::vector<std::vector<int>> clusters(k);
    for (int i = 0; i < n; i++) {
        int c = assignments[i];
        clusters[c].push_back(cities[i]->clef);
    }
    return clusters;
}
