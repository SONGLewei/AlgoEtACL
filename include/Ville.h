#pragma once

#include <string>

using namespace std;

class Ville {
  private:
    string nom;
    double latitude;
    double longitude;

  public:
    Ville(string nom, double latitude, double longitue);
    double distance(Ville v);
}