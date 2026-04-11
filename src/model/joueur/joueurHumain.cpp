#include "JoueurHumain.h"
#include <iostream>

JoueurHumain::JoueurHumain(const std::string& nom, Couleur couleur)
    : Joueur(nom, couleur) {
}

void JoueurHumain::jouer() {
    std::cout << nom << " joue (humain)" << std::endl;
}