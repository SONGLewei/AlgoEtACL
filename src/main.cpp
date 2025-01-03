#include "../includeAlgo/Christo.h"
#include "../includeGraphe/GrapheAll.h"

int main(){

    JSONLoader loader;
    auto grapheGR = loader.chargerGrapheFromJSON("../13_fichiers_JSON/GrandEst.json");
    /*auto grapheAU= loader.chargerGrapheFromJSON("../13_fichiers_JSON/Auvergne-Rhone-Alpes.json");
    auto grapheBO = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Bourgogne-Franche-Comte.json");
    auto grapheBR = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Bretagne.json");
    auto grapheCE = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Centre-Val_de_Loire.json");
    auto grapheCO = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Corse.json");
    auto grapheHA = loader.chargerGrapheFromJSON("../13_fichiers_JSON/HautDeFrance.json");
    auto grapheIL = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Ile-de-France.json");
    auto grapheNO = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Normandie.json");
    auto grapheNU = loader.chargerGrapheFromJSON("../13_fichiers_JSON/NouvelleAquitaine.json");
    auto grapheOC = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Occitanie.json");
    auto graphePA = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Pays_de_la_Loire.json");
    auto graphePR = loader.chargerGrapheFromJSON("../13_fichiers_JSON/Provence_Alpes_Cote_Azur.json");*/

    auto clusters = KMeansPP(grapheGR, 2);

    //std::vector<std::vector<std::string>> truckRoutes = getAllTruckRoutes(grapheGR, clusters);
    auto [maxCmax, cmaxPath] = getMaxCmaxWithPath(grapheGR, clusters);

    auto allClusterRoutes = getAllClusterRoutes(grapheGR, clusters);

    return 0;
}