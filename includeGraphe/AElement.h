#pragma once

#include <string>

using namespace std;

class AElement {
  private:
    static int nextKey;
  public://Pour simplier le projet on peut mettre comme public
    int clef;//Pour stocker une seul cle

    AElement()
      : clef(nextKey++){}
    
    operator string() const {
      return "AElement: " + to_string(clef);
    }

    friend ostream& operator<< (ostream& os, const AElement& element) {
      return os << string(element);
    }
};

int AElement::nextKey = 0;