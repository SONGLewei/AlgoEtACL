#pragma once

#include <string>

using namespace std;

class AElement {
  public://Pour simplier le projet on peut mettre comme public
    int clef;//Pour stocker une seul cle

    AElement(int clef) : clef(clef) {};
    
    operator string() const {
      return "AElement: " + to_string(clef);
    }

    friend ostream& operator<< (ostream& os, const AElement& element) {
      return os << string(element);
    }
};