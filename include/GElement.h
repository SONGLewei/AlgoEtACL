#pragma once

#include <string>
#include "AElement.h"

using namespace std;
//Pour avoir plusieur type de T(entree peut etre int ou string)
template <class T>
class GElement : public AElement {
  public:
    T v;

    GElement(int clef, T v) : AElement(clef), v(v) {};

    string valueToString() const {
      if constexpr (is_same<T, string>::value) {
        return v;
      } else {
        return to_string(v);
      }
    }
    
    operator string() const {
      return "GElement: " + AElement::operator string() + ", Value: " + valueToString();
    }

    friend ostream& operator<< (ostream& os, const GElement& element) {
      return os << string(element);
    }
};