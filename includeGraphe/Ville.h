#pragma once

#include <string>

using namespace std;

class Ville {
  public:
    string name;
    double latitude;
    double longitude;

    Ville(): name(""),latitude(0.0),longitude(0.0){}
    Ville(const string& n,double lat,double lon):
      name(n),latitude(lat),longitude(lon){}
};