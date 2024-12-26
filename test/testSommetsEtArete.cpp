#include <iostream>
#include "../include/Sommet.h"
#include "../include/Arete.h"

int main() {
    Sommet<string> s1(1, "CityA");
    Sommet<string> s2(2, "CityB");

    Arete<char, string> edge(1, 'A', &s1, &s2);

    cout << edge << endl; 

    cout << "Degree of s1: " << s1.operator string() << endl;
    cout << "Degree of s2: " << s2.operator string() << endl;

    bool isEqual = edge.estEgal(&s1, &s2);
    cout << "Edge connects s1 and s2: " << (isEqual ? "Yes" : "No") << endl;

    return 0;
}
