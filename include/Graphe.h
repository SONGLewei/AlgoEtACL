#pragma once

#include "Sommet.h"
#include "Arete.h"

using namespace std;

class Graphe {
  private:
    PElement<Sommet<T>> sommets;
    PElement<Arete<S,T>> aretes;

  public:
    Graphe();
    ~Graphe();

    int nombreSommets();
    int nombreAretes();
    Sommet<T> creerSommet(T info);
    Arete<S,T> creerArete(S info, Sommet<T> debut, Sommet<T> fin);
    PElement<Arete<S,T>> aretesAdjacentes(Sommet<T> s);
    PElement<Sommet<T>> voisisns(Sommet<T> s);
    Arete trouverArete(Sommet<S> debut, Sommet<S> fin);

    string toString();
}