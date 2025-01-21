
#include "../../include/Graphe/BuildNewGraphe.h"

Graphe<double, Ville> buildNewGrapheWithPaths(
    const Graphe<double, Ville>& graphe,
    const std::vector<std::vector<int>>& clusters,
    const std::vector<std::pair<double, std::vector<std::string>>>& results)
{
    Graphe<double, Ville> newGraphe;

    // （1）先将原图里的城市复制到新图中
    //      这里用 nameToOldSommet 和 nameToNewSommet 做对应关系
    // (1) Copiez d'abord les villes de l'image originale dans la nouvelle image
    // Utilisez ici nameToOldSommet et nameToNewSommet pour établir la relation correspondante
    std::unordered_map<std::string, Sommet<Ville>*> nameToOldSommet;
    for (auto it = graphe.lSommets; it; it = it->suivant) {
        Sommet<Ville>* oldCity = it->valeur;
        nameToOldSommet[oldCity->v.name] = oldCity;
    }

    // 在新图中创建同名城市
    //Créer une ville du même nom dans la nouvelle image
    std::unordered_map<std::string, Sommet<Ville>*> nameToNewSommet;
    for (auto& kv : nameToOldSommet) {
        auto oldCity = kv.second;
        auto newCity = newGraphe.creeSommet(oldCity->v);
        nameToNewSommet[oldCity->v.name] = newCity;
    }

    // （2）根据各簇的路径，为“相邻”城市连边
    //      results[i].second 就是第 i 个簇的路径（城市名字按访问顺序排列），例如 [CityA, CityB, CityC, CityA]
    // (2) En fonction des chemins de chaque cluster, connecter les bords des villes "adjacentes"
    // results[i].second est le chemin d'accès au i-ème cluster (les noms des villes sont classés par ordre d'accès), par exemple [CityA, CityB, CityC, CityA]
    for (const auto& [cmax, path] : results) {
        // path.size()-1
        for (size_t i = 0; i + 1 < path.size(); i++) {
            const std::string& cityName1 = path[i];
            const std::string& cityName2 = path[i + 1];

            // 从旧图里获取对应的距离
            // Récupère la distance correspondante par rapport à l'ancienne image
            auto oldS1 = nameToOldSommet[cityName1];
            auto oldS2 = nameToOldSommet[cityName2];
            Arete<double, Ville>* oldEdge = graphe.getAreteParSommets(oldS1, oldS2);
            if (!oldEdge) {
                continue;
            }

            double dist = oldEdge->v;

            // 在新图里找到对应的节点
            // Trouver le nœud correspondant dans le nouveau graphique
            auto newS1 = nameToNewSommet[cityName1];
            auto newS2 = nameToNewSommet[cityName2];

            // 在新图中创建这条边
            //Créez cette arête dans un nouveau graphe
            newGraphe.creeArete(dist, newS1, newS2);
        }
    }

    return newGraphe;
}