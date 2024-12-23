#pragma once

#include "GElement.h"
#include "Ville.h"

using namespace std;

class Sommet : public GElement<Ville> {
  private:
    int degre;

  public:
    Sommet(int clef, T v);
    void augmenterDegre();
    void diminuerDegre();
    string toString();
}