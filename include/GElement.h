#pragma once

#include <string>
#include "AElement.h"

using namespace std;

class GElement : public AElement {
  private:
    T v;

  public:
    GElement(int clef, T v);
    string toString();
};