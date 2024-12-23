#include <iostream>
#include "AElement.h"
#include "GElement.h"
#include "Graphe.h"

using namespace std;

int main() {
  // Test AElement
  cout << "Test AElement" << endl;
  AElement a(1);
  cout << a << endl;

  // Test GElement
  cout << "Test GElement" << endl;
  GElement<int> g(2, 3);
  cout << g << endl;

  // Test Sommet<double>
  cout << "Test Sommet<double>" << endl;
  Sommet<double> sommetDouble(3, 4.5);
  cout << sommetDouble << endl;

  // Test Sommet<string>
  cout << "Test Sommet<string>" << endl;
  Sommet<string> sommetString(4, "Hello");
  cout << sommetString << endl;

  // Test Arete<double, double>
  cout << "Test Arete<double, double>" << endl;
  Sommet<double> sommetDouble1(5, 6.7);
  Sommet<double> sommetDouble2(6, 7.8);
  Arete<double, double> areteDouble(7, 8.9, &sommetDouble1, &sommetDouble2);
  cout << areteDouble << endl;

  // Test Arete<string, string>
  cout << "Test Arete<string, string>" << endl;
  Sommet<string> sommetString1(8, "World");
  Sommet<string> sommetString2(9, "!");
  Arete<string, string> areteString(10, "Hello", &sommetString1, &sommetString2);
  cout << areteString << endl;

  cout << "All tests passed" << endl;

  return 0;
}