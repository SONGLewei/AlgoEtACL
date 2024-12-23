#pragma once

#include "PElement.h"
#include "Sommet.h"
#include "Arete.h"

using namespace std;

template <class S, class T>
class Graphe {
  private:
    PElement<Sommet<T>>* lSommets;
    PElement<Arete<S,T>>* lAretes;

    Sommet<T>* getSommetParClef(int clef) const {
      for (PElement<Sommet<T>>* p = lSommets; p; p = p->suivant) {
        if (p->v->clef == clef) {
          return p->v;
        }
      }
      return nullptr;
    }

  public:
    Graphe() : lSommets(nullptr), lAretes(nullptr) {}

    Graphe(const Graphe<S, T>& graphe) : lSommets(nullptr), lAretes(nullptr) {
      for (PElement<Sommet<T>>* p = graphe.lSommets; p; p = p->suivant) {
        creerSommet(p->v->info);
      }
      for (PElement<Arete<S, T>>* p = graphe.lAretes; p; p = p->suivant) {
        creerArete(p->v->info, getSommetParClef(p->v->debut->clef), getSommetParClef(p->v->fin->clef));
      }
    }

    ~Graphe() {
      delete lSommets;
      delete lAretes;
    }

    int nombreSommets() {
      return PElement<Sommet<T>>::taille(lSommets);
    }

    int nombreAretes() {
      return PElement<Arete<S,T>>::taille(lAretes);
    }

    Sommet<T> * creerSommet(const T& info) {
      Sommet<T> * sommet = new Sommet<T>(nombreSommets(), info);
      lSommets = new PElement<Sommet<T>>(sommet, lSommets);
      return sommet;
    }

    Arete<S,T> * creerArete(const S& info, Sommet<T> * debut, Sommet<T> * fin) {
      Arete<S,T> * arete = new Arete<S,T>(nombreAretes(), info, debut, fin);
      lAretes = new PElement<Arete<S,T>>(arete, lAretes);
      return arete;
    }

    PElement<pair<Sommet<T>*, Arete<S,T>*>>* adjacences(const Sommet<T> * sommet) const {
      PElement<pair<Sommet<T>*, Arete<S, T>> >* adj = nullptr;
      for (PElement<Arete<S, T>>* p = lAretes; p; p = p->suivant) {
        if (p->v->debut == sommet) {
          adj = new PElement<pair<Sommet<T>*, Arete<S, T>>>(make_pair(p->v->fin, p->v), adj);
        } else if (p->v->fin == sommet) {
          adj = new PElement<pair<Sommet<T>*, Arete<S, T>>>(make_pair(p->v->debut, p->v), adj);
        }
      }
      return adj;
    }

    PElement<Arete<S,T>> * aretesAdjacentes(Sommet<T> * s) {
      PElement<Arete<S, T>>* adj = nullptr;
      for (PElement<Arete<S, T>>* p = lAretes; p; p = p->suivant) {
        if (p->v->debut == s || p->v->fin == s) {
          adj = new PElement<Arete<S, T>>(p->v, adj);
        }
      }
      return adj;
    }

    PElement<Sommet<T>> * voisins(Sommet<T> * s) {
      PElement<Sommet<T>>* voisins = nullptr;
      for (PElement<pair<Sommet<T>*, Arete<S, T>> >* p = adjacences(s); p; p = p->s) {
        voisins = new PElement<Sommet<T>>(p->v, voisins);
      }
      return voisins;
    }

    Arete<S,T> * getAreteParSommets(Sommet<S> * s1, Sommet<S> * s2) {
      for (PElement<Arete<S, T>>* p = lAretes; p; p = p->suivant) {
        if (p->v->estEgal(s1, s2)) {
          return p->v;
        }
      }
      return nullptr;
    }

    operator string() const {
      return "Graphe: " + to_string(nombreSommets()) + " sommets, " + to_string(nombreAretes()) + " aretes";
    }

    friend ostream& operator<<(ostream& os, const Graphe<S, T>& graphe) {
      return os << string(graphe);
    }

    const Graphe<S, T>& operator=(const Graphe<S, T>& graphe) {
      if (this != &graphe) {
        delete lSommets;
        delete lAretes;
        lSommets = nullptr;
        lAretes = nullptr;
        for (PElement<Sommet<T>>* p = graphe.lSommets; p; p = p->suivant) {
          creerSommet(p->v->info);
        }
        for (PElement<Arete<S, T>>* p = graphe.lAretes; p; p = p->suivant) {
          creerArete(p->v->info, getSommetParClef(p->v->debut->clef), getSommetParClef(p->v->fin->clef));
        }
      }
      return *this;
    }
};