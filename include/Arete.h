#pragma once

#include "GElement.h"
#include "Sommet.h"

using namespace std;

template <class S, class T>
class Arete : public GElement<S> {
  private:
    Sommet<T> * debut;
    Sommet<T> * fin;

  public:
    Arete(int clef, S v, Sommet<T> * debut, Sommet<T> * fin) : GElement<S>(clef, v),debut(debut), fin(fin) {
      debut->augmenterDegre();
      fin->augmenterDegre();
    }

    ~Arete() {
      debut->diminuerDegre();
      fin->diminuerDegre();
    }

    bool estEgal(Sommet<T> * s1, Sommet<T> * s2) {
      return (debut == s1 && fin == s2) || (debut == s2 && fin == s1);
    }

    operator string() const {
      return "Arete: " + debut->operator std::string() + "," + fin->operator std::string();
    }

    friend ostream& operator<<(ostream& os, const Arete<S, T>& arete) {
      return os << string(arete);
    }
};