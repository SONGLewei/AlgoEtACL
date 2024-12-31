#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>

#include "PElement.h"
#include "Sommet.h"
#include "Arete.h"
#include "Ville.h"

/**
 * @brief Classe template Graphe<S, T> : 
 *  - S 代表边的权值类型（如 double, int 等）
 *  - T 代表顶点信息类型（如 Ville）。
 */
template <class S, class T>
class Graphe {
public:
    // Pointeurs vers listes chaînées de sommets et d'arêtes
    PElement<Sommet<T>>* lSommets;
    PElement<Arete<S, T>>* lAretes;

    /**
     * @brief Constructeur par défaut
     */
    Graphe()
        : lSommets(nullptr), lAretes(nullptr)
    {}

    /**
     * @brief Constructeur de copie
     */
    Graphe(const Graphe<S, T>& other)
        : lSommets(PElement<Sommet<T>>::copier(other.lSommets)),
          lAretes(PElement<Arete<S, T>>::copier(other.lAretes))
    {}

    /**
     * @brief Destructeur
     */
    ~Graphe() {
        libererListeAretes();
        libererListeSommets();
    }

    /**
     * @brief Opérateur d'affectation
     */
    Graphe<S, T>& operator=(const Graphe<S, T>& other) {
        if (this != &other) {
            libererListeSommets();
            libererListeAretes();
            lSommets = PElement<Sommet<T>>::copier(other.lSommets);
            lAretes = PElement<Arete<S, T>>::copier(other.lAretes);
        }
        return *this;
    }

    /**
     * @brief Création et ajout d'un nouveau sommet au graphe
     * @param info L'information portée par le sommet (par ex. Ville)
     * @return Pointeur vers le nouveau sommet créé
     */
    Sommet<T>* creeSommet(const T& info) {
        Sommet<T>* nouveauSommet = new Sommet<T>(info);
        lSommets = PElement<Sommet<T>>::ajouter(lSommets, nouveauSommet);
        return nouveauSommet;
    }

    /**
     * @brief Supprime un sommet donné, et toutes les arêtes associées
     * @param sommet Le sommet à supprimer
     */
    void supprimerSommet(Sommet<T>* sommet) {
        // 1. Supprimer toutes les arêtes liées à ce sommet
        PElement<Arete<S, T>>* it = lAretes;
        while (it) {
            Arete<S, T>* currentArete = it->valeur;
            it = it->suivant;
            if (currentArete->debut == sommet || currentArete->fin == sommet) {
                lAretes = PElement<Arete<S, T>>::retirer(lAretes, currentArete);
            }
        }
        // 2. Retirer le sommet de la liste des sommets
        lSommets = PElement<Sommet<T>>::retirer(lSommets, sommet);
    }

    /**
     * @brief Création et ajout d'une nouvelle arête au graphe
     * @param info  La valeur (poids) de l'arête (ex: distance)
     * @param debut Pointeur vers le sommet de début
     * @param fin   Pointeur vers le sommet de fin
     * @return Pointeur vers la nouvelle arête créée
     */
    Arete<S, T>* creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin) {
        Arete<S, T>* nouvelleArete = new Arete<S, T>(info, debut, fin);
        lAretes = PElement<Arete<S, T>>::ajouter(lAretes, nouvelleArete);
        return nouvelleArete;
    }

    /**
     * @brief Supprime une arête du graphe
     * @param arete L'arête à supprimer
     */
    void supprimerArete(Arete<S, T>* arete) {
        lAretes = PElement<Arete<S, T>>::retirer(lAretes, arete);
    }

    /**
     * @brief Renvoie le nombre de sommets
     */
    int nombreSommets() const {
        return PElement<Sommet<T>>::longueur(lSommets);
    }

    /**
     * @brief Renvoie le nombre d'arêtes
     */
    int nombreAretes() const {
        return PElement<Arete<S, T>>::longueur(lAretes);
    }

    /**
     * @brief Renvoie la liste des arêtes adjacentes à un sommet donné
     */
    PElement<Arete<S, T>>* aretesAdjacentes(const Sommet<T>* sommet) const {
        PElement<Arete<S, T>>* resultat = nullptr;
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            if (it->valeur->debut == sommet || it->valeur->fin == sommet) {
                resultat = PElement<Arete<S, T>>::ajouter(resultat, it->valeur);
            }
        }
        return resultat;
    }

    /**
     * @brief Renvoie la liste des sommets voisins d'un sommet
     */
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

    /**
     * @brief Recherche d'une arête selon deux sommets
     * @param s1 Premier sommet
     * @param s2 Deuxième sommet
     * @return Pointeur vers l'arête correspondante, ou nullptr si inexistante
     */
    Arete<S, T>* getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const {
        for (PElement<Arete<S, T>>* it = lAretes; it; it = it->suivant) {
            if (it->valeur->estEgal(
                    const_cast<Sommet<T>*>(s1), 
                    const_cast<Sommet<T>*>(s2))) {
                return it->valeur;
            }
        }
        return nullptr;
    }

    /**
     * @brief Convertit le graphe en chaîne de caractères (pour l'affichage)
     */
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

    /**
     * @brief extractSubgraph : 
     *        Selon un ensemble de noms de Ville（cityNames），
     *        on crée un sous-graphe contenant uniquement 
     *        ces villes及其之间的边。
     * @param cityNames Unordered_set contenant les noms des villes
     * @return Le sous-graphe correspondant
     *
     * @note 该方法仅在 T=Ville 的情况下有效。使用 static_assert 确保类型正确。
     */
    Graphe<S, T> extractSubgraph(const std::unordered_set<std::string>& cityNames) const {
        // 仅当 T = Ville 时可用，否则编译时报错
        static_assert(std::is_same<T, Ville>::value,
                      "extractSubgraph is only valid when T = Ville.");

        Graphe<S, T> subgraph;

        // 构建映射：旧顶点 -> 新顶点，以便在复制边时使用正确的指针
        std::unordered_map<const Sommet<T>*, Sommet<T>*> mapping;

        // 1. 复制在 cityNames 中出现的所有顶点
        for (auto it = lSommets; it != nullptr; it = it->suivant) {
            const Sommet<T>* oldSommet = it->valeur;
            if (cityNames.find(oldSommet->v.name) != cityNames.end()) {
                // 在子图中创建对应的节点
                Sommet<T>* newSommet = subgraph.creeSommet(oldSommet->v);
                mapping[oldSommet] = newSommet;
            }
        }

        // 2. 复制边：仅当边的两个端点都在 mapping 中时
        for (auto it = lAretes; it != nullptr; it = it->suivant) {
            Arete<S, T>* oldArete = it->valeur;
            const Sommet<T>* sDebut = oldArete->debut;
            const Sommet<T>* sFin   = oldArete->fin;

            if (mapping.find(sDebut) != mapping.end() &&
                mapping.find(sFin)   != mapping.end()) 
            {
                subgraph.creeArete(
                    oldArete->v,
                    mapping[sDebut],
                    mapping[sFin]
                );
            }
        }
        return subgraph;
    }

private:
    /**
     * @brief libère la mémoire de la liste de sommets
     */
    void libererListeSommets() {
        while (lSommets) {
            PElement<Sommet<T>>* temp = lSommets->suivant;
            delete lSommets;  // 这里会调用 PElement<Sommet<T>>::~PElement
            lSommets = temp;
        }
    }

    /**
     * @brief libère la mémoire de la liste d'arêtes
     */
    void libererListeAretes() {
        while (lAretes) {
            PElement<Arete<S, T>>* temp = lAretes->suivant;
            delete lAretes;   // 这里会调用 PElement<Arete<S, T>>::~PElement
            lAretes = temp;
        }
    }
};

