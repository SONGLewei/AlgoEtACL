#pragma once

#include "GElement.h"
#include "Ville.h"

using namespace std;

template <class T>
class Sommet : public GElement<T> {
  public:
    int degre;

    Sommet(T v) 
      :GElement<T>(v), degre(0) {}

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