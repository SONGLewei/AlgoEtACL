
#include "../../include/Graphe/BuildNewGraphe.h"

/**
 * 根据聚类结果和每个簇的贪心路径，在新图中仅创建必要的城市与边。
 * @param graphe   : 原图（包含所有城市以及城市间距离）
 * @param clusters : 每个簇中城市的 key 列表（如 KMeansPP 返回的结果）
 * @param results  : 对应每个簇的 (Cmax, 路径)；路径里是城市的字符串名字，顺序为贪心算法计算得到的顺序
 * @return         : 一个包含所有城市节点，但仅包含各簇路径上相邻城市的边的“新图”
 */
Graphe<double, Ville> buildNewGrapheWithPaths(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters,
    const std::vector<std::pair<double, std::vector<std::string>>>& results)
{
    Graphe<double, Ville> newGraphe;

    // （1）先将原图里的城市复制到新图中
    //      这里用 nameToOldSommet 和 nameToNewSommet 做对应关系
    std::unordered_map<std::string, Sommet<Ville>*> nameToOldSommet;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        Sommet<Ville>* oldCity = it->valeur;
        nameToOldSommet[oldCity->v.name] = oldCity;
    }

    // 在新图中创建同名城市（注意这时还没有连边）
    std::unordered_map<std::string, Sommet<Ville>*> nameToNewSommet;
    for (auto& kv : nameToOldSommet) {
        auto oldCity = kv.second;
        auto newCity = newGraphe.creeSommet(oldCity->v);
        nameToNewSommet[oldCity->v.name] = newCity;
    }

    // （2）根据各簇的路径，为“相邻”城市连边
    //      results[i].second 就是第 i 个簇的路径（城市名字按访问顺序排列），例如 [CityA, CityB, CityC, CityA]
    for (const auto& [cmax, path] : results) {
        // path.size()-1 次相邻边
        for (size_t i = 0; i + 1 < path.size(); i++) {
            const std::string& cityName1 = path[i];
            const std::string& cityName2 = path[i + 1];

            // 从旧图里获取对应的距离
            auto oldS1 = nameToOldSommet[cityName1];
            auto oldS2 = nameToOldSommet[cityName2];
            Arete<double, Ville>* oldEdge = graphe.getAreteParSommets(oldS1, oldS2);
            if (!oldEdge) {
                // 如果在原图里没找到这两城市的直接边（极少见，因为你说了“每个城市之间都有路”），跳过
                continue;
            }

            double dist = oldEdge->v; // 公里数

            // 在新图里找到对应的节点
            auto newS1 = nameToNewSommet[cityName1];
            auto newS2 = nameToNewSommet[cityName2];

            // 在新图中创建这条边（无向图 -> 可视需要看是否重复创建）
            newGraphe.creeArete(dist, newS1, newS2);
        }
    }

    return newGraphe;
}