#include <iostream>
#include <string>
#include "../includeGraphe/Graphe.h"

using namespace std;

int main() {
    //Initialisation d'un graphe
    Graphe<char, string> graphe;

    //Ici pour créer les sommets ou le ville sur le projet
    auto* s1 = graphe.creeSommet("CityA");
    auto* s2 = graphe.creeSommet("CityB");
    auto* s3 = graphe.creeSommet("CityC");
    auto* s4 = graphe.creeSommet("CityD");

    //creer les Aretes
    auto* a1 = graphe.creeArete('a', s1, s2);
    auto* a2 = graphe.creeArete('b', s2, s3);
    auto* a3 = graphe.creeArete('c', s3, s4);
    auto* a4 = graphe.creeArete('d', s4, s1);
    auto* a5 = graphe.creeArete('e', s1, s3);

    cout << "=== Graph Information ===" << endl;
    cout << graphe << endl;

    //get all the sommets de graphe
    cout << "Number of sommets: " << graphe.nombreSommets() << endl;
    cout << "Number of aretes: " << graphe.nombreAretes() << endl;

    //Pour voir les aretes d'adjacents
    cout << "\nAretes adjacent to CityA:" << endl;
    auto adjEdges = graphe.aretesAdjacentes(s1);
    PElement<Arete<char, string>>::afficher(adjEdges);

    //Voir les autres villes adjacents
    cout << "\nSommets adjacent to CityA:" << endl;
    auto voisins = graphe.voisins(s1);
    PElement<Sommet<string>>::afficher(voisins);

    cout << "\nArete between CityA and CityC:" << endl;
    auto edge = graphe.getAreteParSommets(s1, s3);
    if (edge) {
        cout << *edge << endl;
    } else {
        cout << "No edge found between CityA and CityC" << endl;
    }

    //pour afficher le graphe clair
    cout << "\n=== Graph Visualization ===" << endl;
    cout << "Sommets: " << endl;
    for (PElement<Sommet<string>>* it = graphe.lSommets; it; it = it->suivant) {
        cout << "  [" << it->valeur->clef << "] " << it->valeur->v << " (degree: " << it->valeur->degre << ")" << endl;
    }

    cout << "Aretes: " << endl;
    for (PElement<Arete<char, string>>* it = graphe.lAretes; it; it = it->suivant) {
        cout << "  " << it->valeur->v << ": " << it->valeur->debut->v 
             << " -> " << it->valeur->fin->v << endl;
    }

    cout << "\n=== Supprimer some parts ===" << endl;

    //On peut supprimer les aretes ou les sommets, ca sera clair
    cout << "\nRemoving Arete a1 (CityA->CityB)..." << endl;
    graphe.supprimerArete(a1);  
    cout << graphe << endl;
    cout << "Sommets: " << graphe.nombreSommets() << ", "
        << "Aretes: "    << graphe.nombreAretes() << endl;

    cout << "\nRemoving sommets CityC..." << endl;
    graphe.supprimerSommet(s3);
    cout << graphe << endl;
    cout << "Sommets: " << graphe.nombreSommets() << ", "
        << "aretes: "    << graphe.nombreAretes() << endl;

    return 0;
}
