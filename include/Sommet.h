#pragma once

#include "GElement.h"
#include "Ville.h"

using namespace std;

template <class T>
class Sommet : public GElement<T> {
  private:
    int degre;

  public:
    Sommet(int clef, T v) : GElement<T>(clef, v), degre(0) {}

    void augmenterDegre() {
      degre++;
    }

    void diminuerDegre() {
      degre--;
    }
    
    operator string() const {
      return "Sommet: " + this->GElement<T>::operator string() + ", Degree: " + to_string(degre);
    }

    friend ostream& operator<< (ostream& os, const Sommet& sommet) {
      return os << string(sommet);
    }
};