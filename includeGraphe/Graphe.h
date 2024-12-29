#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include "PElement.h"
#include "Sommet.h"
#include "Arete.h"

template <class S, class T>
class Graphe {
public:
    PElement<Sommet<T>>* lSommets;
    PElement<Arete<S, T>>* lAretes;
    int prochaineClef;

    Graphe() : lSommets(nullptr), lAretes(nullptr), prochaineClef(0) {}

    Graphe(const Graphe<S, T>& other)
        : lSommets(PElement<Sommet<T>>::copier(other.lSommets)),
          lAretes(PElement<Arete<S, T>>::copier(other.lAretes)),
          prochaineClef(other.prochaineClef) {}

    ~Graphe() {
        libererListeSommets();
        libererListeAretes();
    }

    Graphe<S, T>& operator=(const Graphe<S, T>& other) {
        if (this != &other) {
            libererListeSommets();
            libererListeAretes();
            lSommets = PElement<Sommet<T>>::copier(other.lSommets);
            lAretes = PElement<Arete<S, T>>::copier(other.lAretes);
            prochaineClef = other.prochaineClef;
        }
        return *this;
    }

    Sommet<T>* creeSommet(const T& info) {
        Sommet<T>* nouveauSommet = new Sommet<T>(prochaineClef++, info);
        lSommets = PElement<Sommet<T>>::ajouter(lSommets, nouveauSommet);
        return nouveauSommet;
    }

    void supprimerSommet(Sommet<T>* sommet) {
        // Supprimer toutes les arêtes liées
        PElement<Arete<S, T>>* it = lAretes;
        while (it) {
            Arete<S, T>* currentArete = it->valeur;
            it = it->suivant;
            if (currentArete->debut == sommet || currentArete->fin == sommet) {
                lAretes = PElement<Arete<S, T>>::retirer(lAretes, currentArete);
            }
        }
        // Supprimer le sommet dans la liste
        lSommets = PElement<Sommet<T>>::retirer(lSommets, sommet);
    }

    Arete<S, T>* creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin) {
        Arete<S, T>* nouvelleArete = new Arete<S, T>(prochaineClef++, info, debut, fin);
        lAretes = PElement<Arete<S, T>>::ajouter(lAretes, nouvelleArete);
        return nouvelleArete;
    }

    void supprimerArete(Arete<S, T>* arete) {
        lAretes = PElement<Arete<S, T>>::retirer(lAretes, arete);
    }

    int nombreSommets() const {
        return PElement<Sommet<T>>::longueur(lSommets);
    }

    int nombreAretes() const {
        return PElement<Arete<S, T>>::longueur(lAretes);
    }

    PElement<Arete<S, T>>* aretesAdjacentes(const Sommet<T>* sommet) const {
        PElement<Arete<S, T>>* resultat = nullptr;
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            if (it->valeur->debut == sommet || it->valeur->fin == sommet) {
                resultat = PElement<Arete<S, T>>::ajouter(resultat, it->valeur);
            }
        }
        return resultat;
    }

    PElement<Sommet<T>>* voisins(const Sommet<T>* sommet) const {
        PElement<Sommet<T>>* resultat = nullptr;
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            if (it->valeur->debut == sommet) {
                resultat = PElement<Sommet<T>>::ajouter(resultat, it->valeur->fin);
            } else if (it->valeur->fin == sommet) {
                resultat = PElement<Sommet<T>>::ajouter(resultat, it->valeur->debut);
            }
        }
        return resultat;
    }

    Arete<S, T>* getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const {
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            if (it->valeur->estEgal(const_cast<Sommet<T>*>(s1), const_cast<Sommet<T>*>(s2))) {
                return it->valeur;
            }
        }
        return nullptr;
    }

    std::string toString() const {
        std::ostringstream oss;
        oss << "Sommets:\n";
        for (PElement<Sommet<T>>* it = lSommets; it; it = it->suivant) {
            oss << "  " << *(it->valeur) << "\n";
        }

        oss << "Aretes:\n";
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            oss << "  " << *(it->valeur) << "\n";
        }

        return oss.str();
    }

    friend std::ostream& operator<<(std::ostream& os, const Graphe<S, T>& graphe) {
        return os << graphe.toString();
    }

    // -----------------------------------------------------------------------
    // **新增函数**: 保留传入的边，删除其他边
    // -----------------------------------------------------------------------
    void supprimerToutesAretesExcept(const std::vector<Arete<S, T>*>& aretesAConserver) {
        std::unordered_set<Arete<S, T>*> setAretToKeep;
        setAretToKeep.insert(aretesAConserver.begin(), aretesAConserver.end());

        PElement<Arete<S, T>>* it = lAretes;
        while (it) {
            Arete<S, T>* courant = it->valeur;
            it = it->suivant;
            if (setAretToKeep.find(courant) == setAretToKeep.end()) {
                supprimerArete(courant);
            }
        }
    }

private:
    void libererListeSommets() {
        while (lSommets) {
            PElement<Sommet<T>>* temp = lSommets->suivant;
            delete lSommets; // delete 也会调用 PElement 析构 => 删除 sommet
            lSommets = temp;
        }
    }

    void libererListeAretes() {
        while (lAretes) {
            PElement<Arete<S, T>>* temp = lAretes->suivant;
            delete lAretes;  // delete 也会调用 PElement 析构 => 删除 arete
            lAretes = temp;
        }
    }
};
