#pragma once

#include <string>

using namespace std;

class AElement {
  private:
    int clef;

  public:
    AElement(int clef);
    string toString();
}