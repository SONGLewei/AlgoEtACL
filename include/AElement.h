#pragma once

#include <string>

using namespace std;

class AElement {
  private:
    int clef;

  public:
    AElement(int clef) : clef(clef) {};
    
    operator string() const {
      return "AElement: " + to_string(clef);
    }

    friend ostream& operator<< (ostream& os, const AElement& element) {
      return os << string(element);
    }
};