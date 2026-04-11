#include "JoueurIA.h"
#include <iostream>

JoueurIA::JoueurIA(const std::string& nom, Couleur couleur, int profondeur)
    : Joueur(nom, couleur), profondeurRecherche(profondeur) {
}

void JoueurIA::jouer() {
    std::cout << nom << " joue (IA)" << std::endl;
}