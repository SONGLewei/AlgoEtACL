#pragma once

#include <string>
#include "AElement.h"

using namespace std;

template <class T>
class GElement : public AElement {
  private:
    T v;

    string valueToString() const {
      if constexpr (is_same<T, string>::value) {
        return v;
      } else {
        return to_string(v);
      }
    }

  public:
    GElement(int clef, T v) : AElement(clef), v(v) {};
    
    operator string() const {
      return "GElement: " + AElement::operator string() + ", Value: " + valueToString();
    }

    friend ostream& operator<< (ostream& os, const GElement& element) {
      return os << string(element);
    }
};