#include "JoueurHumain.h"
#include <iostream>

// Constructeur : appelle le constructeur de la classe parente Joueur
JoueurHumain::JoueurHumain(const std::string& nom, Couleur couleur)
    : Joueur(nom, couleur) {
}

// Le joueur humain joue : affiche un message (la logique complète sera dans le contrôleur)
void JoueurHumain::jouer() {
    std::cout << nom << " joue (humain)" << std::endl;
}