
#include "../../include/Algo/Kmeans.h"


std::vector<Sommet<Ville>*> getAllCities(const Graphe<double, Ville>& graphe) {
    std::vector<Sommet<Ville>*> cities;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        cities.push_back(it->valeur);
    }
    return cities;
}


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


double distanceCityToCity(const Graphe<double, Ville>& graphe,
                          const Sommet<Ville>* c1,
                          const Sommet<Ville>* c2)
{
    Arete<double, Ville>* arete = graphe.getAreteParSommets(c1, c2);
    if (arete) {
        return arete->v;
    }
    return std::numeric_limits<double>::max();
}


std::vector<int> kmeansPlusPlusInit(const Graphe<double, Ville>& graphe,
                                    const std::vector<Sommet<Ville>*>& cities,
                                    int k)
{
    std::vector<int> centers;
    centers.reserve(k);

    if ((int)cities.size() <= k) {
        //Si <k alors tous les villes sont la centre ,et fini
        for (int i = 0; i < (int)cities.size(); i++) {
            centers.push_back(i);
        }
        return centers;
    }
    
    static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));

    //Random choisir un centre
    std::uniform_int_distribution<> distIndex(0, (int)cities.size() - 1);
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

        //按照 “D(x)^2 / sum(D(x)^2)” 的分布来随机选一个点做新中心
        // Sélectionnez aléatoirement un point comme nouveau centre en fonction de la distribution de "D(x)^2 / sum(D(x)^2)"
        double sumD2 = 0.0;
        for (double d : minDistances) {
            sumD2 += (d * d);
        }

        std::uniform_real_distribution<> distReal(0.0, sumD2);
        double r = distReal(gen);

        double cumulative = 0.0;
        int chosenIndex = -1;
        for (int i = 0; i < (int)cities.size(); i++) {
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


std::vector<std::vector<int>> KMeansPP(const Graphe<double, Ville>& graphe, int k)
{
    //0) Collecter la liste des villes
    std::vector<Sommet<Ville>*> cities = getAllCities(graphe);
    int n = (int)cities.size();
    if (n == 0 || k <= 0) {
        return {};
    }

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

    std::vector<int> assignments(n, -1); //记录每个城市属于哪个簇 Enregistrez à quel cluster appartient chaque ville
    bool changed = true;
    int maxIterations = 100;

    static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));

    while (changed && maxIterations-- > 0) {
        changed = false;

        // a)assignment step:Chaque ville a la plus proche centre(给每个城市分配到最近的中心)
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

        //b) update step: Recalculez le centroïde de chaque cluster (si un cluster est vide, sélectionnez au hasard une nouvelle ville comme centroïde)
        //Pour éviter qu'aucune ville ne se trouve dans une agglomération.
        //重新计算各簇的质心(若某簇为空，则随机挑一个新城市作为质心)
        std::vector<double> sumLat(k, 0.0);
        std::vector<double> sumLon(k, 0.0);
        std::vector<int> count(k, 0);

        for (int i = 0; i < n; i++) {
            int c = assignments[i];
            sumLat[c] += cities[i]->v.latitude;
            sumLon[c] += cities[i]->v.longitude;
            count[c]++;
        }

        for (int c = 0; c < k; c++) {
            if (count[c] > 0) {
                //update la centre
                double newLat = sumLat[c] / count[c];
                double newLon = sumLon[c] / count[c];
                //Déterminer si le changement est supérieur à un certain seuil  判断是否变化大于一定阈值
                if (std::fabs(newLat - centerLats[c]) > 1e-7 ||
                    std::fabs(newLon - centerLons[c]) > 1e-7) {
                    centerLats[c] = newLat;
                    centerLons[c] = newLon;
                }
            } else {
                //S'il n'y a pas de ville dans un cluster, sélectionnez au hasard une ville comme nouveau centroïde. 如果某个簇没有城市，则随机挑一座城市当新的质心
                std::uniform_int_distribution<> distIndex(0, n - 1);
                int randomIndex = distIndex(gen);
                centerLats[c] = cities[randomIndex]->v.latitude;
                centerLons[c] = cities[randomIndex]->v.longitude;
            }
        }
    }

    std::vector<std::vector<int>> clusters(k);
    for (int i = 0; i < n; i++) {
        int c = assignments[i];
        clusters[c].push_back(cities[i]->clef);
    }
    return clusters;
}
