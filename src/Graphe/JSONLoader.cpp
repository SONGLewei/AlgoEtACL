
#include "../../include/Graphe/JSONLoader.h"


    Graphe<double,Ville> JSONLoader::chargerGrapheFromJSON(const string& fichier){
        Graphe<double,Ville> graphe;

        //Pour ouvrir le fichier
        std::ifstream fin(fichier);
        if(!fin.is_open()){
            std::cerr<<"Erruer: impossible open the fichier "<<fichier<<std::endl;
            return graphe;
        }
        
        json j;
        fin>> j;
        fin.close();

        std::vector<Sommet<Ville>*> sommets;
        sommets.reserve(j.size());

        //Creer les ville
        for(auto& item : j){
            std::string ville = item.value("ville","");
            double latitude   = item.value("latitude",0.0);
            double longitude  = item.value("longitude",0.0);

            Ville villeObj(ville,latitude,longitude);

            auto* s =graphe.creeSommet(villeObj);
            sommets.push_back(s);
        }

        for(size_t i = 0;i<sommets.size();++i){
            for(size_t j = i+1;j<sommets.size();++j){
                Ville& v1 = sommets[i]->v;
                Ville& v2 = sommets[j]->v;

                double dist = this->haversineDistance(v1.latitude,v1.longitude,v2.latitude,v2.longitude);

                graphe.creeArete(dist,sommets[i],sommets[j]);
            }
        }

        return graphe;

    }

    double JSONLoader::haversineDistance(double lat1, double lon1,
                             double lat2, double lon2)
    {
        static const double R = 6371.0;
        auto toRad = [](double deg) {
            return deg * 3.14159 / 180.0;
        };

        double dLat = toRad(lat2 - lat1);
        double dLon = toRad(lon2 - lon1);

        double a = std::sin(dLat/2) * std::sin(dLat/2)
                 + std::cos(toRad(lat1)) * std::cos(toRad(lat2))
                 * std::sin(dLon/2) * std::sin(dLon/2);

        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
        double dist = R * c;
        return dist;
    }