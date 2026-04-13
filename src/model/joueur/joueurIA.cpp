#include "JoueurIA.h"
#include <iostream>

// Constructeur : initialise via la classe parente et stocke la profondeur de recherche
JoueurIA::JoueurIA(const std::string& nom, Couleur couleur, int profondeur)
    : Joueur(nom, couleur), profondeurRecherche(profondeur) {
}

// Le joueur IA joue : affiche un message (l'algorithme IA sera implémenté ici)
void JoueurIA::jouer() {
    std::cout << nom << " joue (IA)" << std::endl;
}