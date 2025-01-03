#include <iostream>
#include "../includeAlgo/Christo.h"

int main() {
    // Load your graph here
    // 加载图数据。
    JSONLoader loader;
    auto graphe = loader.chargerGrapheFromJSON("../13_fichiers_JSON/GrandEst.json");

    for(int k = 1; k<8;k++){
    auto clusters = KMeansPP(graphe, k);

    assert((int)clusters.size() == k);
    std::cout << "K-Means++ 分簇数量：" << clusters.size() << "\n";

    int totalAssignedCities = 0;
    for (const auto& cluster : clusters) {
        totalAssignedCities += (int)cluster.size();
    }
    
    assert(totalAssignedCities == graphe.nombreSommets());
    std::cout << "所有城市均被分配到簇中。\n";

    std::vector<Sommet<Ville>*> allCities = getAllCities(graphe);
    std::unordered_map<int, Sommet<Ville>*> keyToCity;
    keyToCity.reserve(allCities.size());
    for (auto* city : allCities) {
        keyToCity[city->clef] = city;
    }

    std::cout << "\n簇分配结果：\n";
    for (size_t i = 0; i < clusters.size(); i++) {
        std::cout << "簇 " << i << ": ";
        for (int cityKey : clusters[i]) {
            auto it = keyToCity.find(cityKey);
            if (it != keyToCity.end()) {
                std::cout << it->second->v.name << " ";
            } else {
                std::cout << "(unknown cityKey=" << cityKey << ") ";
            }
        }
        std::cout << "\n";
    }

    // Calculate and print the maximum Cmax among all clusters
    // 计算并输出所有簇中的最大 Cmax。
    double maxCmax = getMaxCmax(graphe, clusters);
    std::cout << "Maximum Cmax among all clusters: " << maxCmax << std::endl;
    std::cout<<" "<<endl;
    std::cout<<" "<<endl;
    std::cout<<" "<<endl;
    }
    return 0;
}