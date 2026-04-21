#pragma once

#include "Joueur.h"

// Joueur contrôlé par une personne réelle
// Les clics de l'utilisateur sont gérés par le ControleurJeu
class JoueurHumain : public Joueur {
public:
    // Crée un joueur humain avec un nom et une couleur
    JoueurHumain(const std::string& nom, Couleur couleur);

    // Affiche un message indiquant que c'est au tour de ce joueur
    void jouer() override;
};