#pragma once

template <class T>
class PElement {
  public:
    T v;
    PElement * suivant;

    PElement(const T& valeur, PElement* suivant = nullptr) : v(valeur), suivant(suivant) {}

    ~PElement() {
      delete suivant;
    }

    static int taille(PElement* liste) {
      int taille = 0;
      while (liste) {
        taille++;
        liste = liste->suivant;
      }
      return taille;
    }

    static void detruire(PElement* liste) {
      while (liste) {
        PElement* temp = liste;
        liste = liste->suivant;
        delete temp;
      }
    }
};