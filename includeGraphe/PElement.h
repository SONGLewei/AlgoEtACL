#pragma once

#include <iostream>

template <class T>
class PElement {
  public:
    T* valeur;
    PElement<T>* suivant;

    PElement(T* valeur, PElement<T>* suivant = nullptr) 
        : valeur(valeur), suivant(suivant) {}

    ~PElement() {
        if (valeur) {
        delete valeur;
        valeur = nullptr;
      }
      suivant = nullptr;
    }

    //ajouter un element a la tete
    static PElement<T>* ajouter(PElement<T>* liste, T* valeur) {
        return new PElement<T>(valeur, liste);
    }

    static PElement<T>* retirer(PElement<T>* liste, T* valeur) {
    PElement<T>* prev = nullptr;
    PElement<T>* curr = liste;
      while (curr) {
          if (curr->valeur == valeur) {
              if (prev) {
                  prev->suivant = curr->suivant;
              } else {
                  liste = curr->suivant;
              }
              curr->suivant = nullptr;
              delete curr;
              break;
          }
          prev = curr;
          curr = curr->suivant;
      }
      return liste;
    }

    static void afficher(const PElement<T>* liste) {
        while (liste) {
            std::cout << *(liste->valeur) << " -> ";
            liste = liste->suivant;
        }
        std::cout << "nullptr" << std::endl;
    }

    static int longueur(const PElement<T>* liste) {
        int count = 0;
        while (liste) {
            count++;
            liste = liste->suivant;
        }
        return count;
    }


    static PElement<T>* copier(const PElement<T>* liste) {
    if (!liste) return nullptr;
    return new PElement<T>(new T(*(liste->valeur)), copier(liste->suivant));
    }

};
