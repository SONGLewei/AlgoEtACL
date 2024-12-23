#pragma once

#include "GElement.h"
#include "Sommet.h"

using namespace std;

class Arete : public GElement {
  private:
    Sommet<T> debut;
    Sommet<T> fin;

  public:
    Arete(int clef, S v, Sommet debut, Sommet fin)
    ~Arete()
    bool estEgal(Arete a);
    string toString();
}