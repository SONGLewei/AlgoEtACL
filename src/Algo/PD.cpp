
#include "../../include/Algo/PD.h"

std::pair<double, std::vector<std::string>>
tspDynamicProgramming(const Graphe<double, Ville>& graphe,
                      const std::vector<Sommet<Ville>*>& clusterCities)
{
    int n = static_cast<int>(clusterCities.size());

    if (n == 0) {
        return {0.0, {}}; 
    } else if (n == 1) {
        return {0.0, {clusterCities[0]->v.name}};
    }

    //Construire une matrice de distance dist[i][j] = distance de clusterCities[i] à clusterCities[j]  构造距离矩阵 dist[i][j] = clusterCities[i] 到 clusterCities[j] 的距离
    std::vector<std::vector<double>> dist(n, std::vector<double>(n, 0.0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0.0;
            } else {
                dist[i][j] = distanceCityToCity(graphe, clusterCities[i], clusterCities[j]);
            }
        }
    }

    //DP 数组定义: dp[mask][i] = 已访问集合为 mask，最后停在 i 城市时的最短路径距离
    //mask 用二进制表示已访问的城市集合，比如 mask = 5 (0b0101) 代表访问了城市 0 和城市 2
    //dp 大小为 2^n * n
    //Définition du tableau DP : dp[mask][i] = L'ensemble visité est le masque, le chemin le plus court lorsque le dernier arrêt est dans la ville i
    //le masque représente la ville visitée en binaire, par exemple, masque = 5 (0b0101) représente que la ville 0 et la ville 2 ont été visitées
    // la taille du dp est de 2 ^ n * n
    std::vector<std::vector<double>> dp(1 << n, std::vector<double>(n, std::numeric_limits<double>::infinity()));

    //Afin de reconstruire le chemin, le "nœud parent" doit être enregistré  为了能重构路径，需要记录“父节点”
    std::vector<std::vector<int>> parent(1 << n, std::vector<int>(n, -1));

    //我们固定从城市 0 出发（也可以选簇里的任意一个做起点）
    //初始状态 dp[1<<0][0] = 0
    //Nous partons toujours de la ville 0 (vous pouvez également choisir n'importe laquelle du cluster comme point de départ)
    //État initial dp[1<<0][0] = 0
    dp[1][0] = 0.0;
    parent[1][0] = 0; 

    //动态规划更新
    //遍历所有可能的子集 mask
    //Mise à jour de la planification dynamique
    //Parcourir tous les masques de sous-ensembles possibles
    for (int mask = 0; mask < (1 << n); mask++) {
        //Énumérer la dernière ville actuelle i (doit être dans le masque) 枚举当前最后一个城市 i（必须在 mask 内）
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) {
                continue;
            }
            double currentCost = dp[mask][i];
            if (std::isinf(currentCost)) {
                continue;
            }
            //Essayez d'ajouter une ville que vous n'avez pas encore visitée j 尝试加入一个还没访问过的城市 j
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) {
                    continue;
                }
                int nextMask = mask | (1 << j);
                double newCost = currentCost + dist[i][j];
                if (newCost < dp[nextMask][j]) {
                    dp[nextMask][j] = newCost;
                    parent[nextMask][j] = i;
                }
            }
        }
    }

    double bestCost = std::numeric_limits<double>::infinity();
    int bestEnd = -1;

    int fullMask = (1 << n) - 1;
    for (int i = 1; i < n; i++) {
        double cost = dp[fullMask][i] + dist[i][0];
        if (cost < bestCost) {
            bestCost = cost;
            bestEnd = i;
        }
    }

    //Reconstruire le chemin optimal (ordre inverse)重构最优路径(逆序)
    std::vector<int> pathIndices;
    pathIndices.reserve(n + 1);

    int curCity = bestEnd;
    int curMask = fullMask;
    while (curCity != -1) {
        pathIndices.push_back(curCity);
        int temp = parent[curMask][curCity];
        if (temp == curCity) {
            //Indique le retour au point de départ (parent[1][0] = 0 lors de l'initialisation) 说明回到起点(初始化时 parent[1][0] = 0)
            break;
        }
        curMask = curMask & ~(1 << curCity);
        curCity = temp;
    }

    std::reverse(pathIndices.begin(), pathIndices.end());

    std::vector<std::string> pathNames;
    pathNames.reserve(pathIndices.size()+1);
    for (int idx : pathIndices) {
        pathNames.push_back(clusterCities[idx]->v.name);
    }

    if (!pathNames.empty()) {
    pathNames.push_back(pathNames[0]);
    }

    return {bestCost, pathNames};
}

std::vector<std::pair<double, std::vector<std::string>>>
calculateCmaxForAllClustersPD(const Graphe<double, Ville>& graphe,
                            const std::vector<std::vector<int>>& clusters)
{
    std::vector<std::pair<double, std::vector<std::string>>> results;
    results.reserve(clusters.size());

    //Faire clef -> Sommet<Ville>* une table de requête pour un accès rapide 把 clef -> Sommet<Ville>* 做一个查询表，方便快速访问
    std::unordered_map<int, Sommet<Ville>*> keyToCity;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        keyToCity[it->valeur->clef] = it->valeur; 
    }

    for (const auto& cluster : clusters) {
        std::vector<Sommet<Ville>*> clusterCities;
        clusterCities.reserve(cluster.size());
        for (int cityKey : cluster) {
            if (keyToCity.find(cityKey) != keyToCity.end()) {
                clusterCities.push_back(keyToCity[cityKey]);
            }
        }
        auto [cmax, pathNames] = tspDynamicProgramming(graphe, clusterCities);
        results.push_back({cmax, pathNames});
    }

    return results;
}