#include <iostream>
#include "../include/Arete.h"
#include "../include/Sommet.h"
#include "../include/PElement.h"

int main() {
    PElement<Sommet<std::string>>* liste = nullptr;

    Sommet<std::string>* s1 = new Sommet<std::string>(1, "CityA");
    Sommet<std::string>* s2 = new Sommet<std::string>(2, "CityB");
    Sommet<std::string>* s3 = new Sommet<std::string>(3, "CityC");

    liste = PElement<Sommet<std::string>>::ajouter(liste, s1);
    liste = PElement<Sommet<std::string>>::ajouter(liste, s2);
    liste = PElement<Sommet<std::string>>::ajouter(liste, s3);

    std::cout << "Initial list:" << std::endl;
    PElement<Sommet<std::string>>::afficher(liste);

    liste = PElement<Sommet<std::string>>::retirer(liste, s2);
    std::cout << "After remove CityB:" << std::endl;
    PElement<Sommet<std::string>>::afficher(liste);

    delete liste;

    return 0;
}
