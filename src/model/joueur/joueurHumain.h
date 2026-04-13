#pragma once

#include "Joueur.h"

// Représente un joueur humain qui joue manuellement
class JoueurHumain : public Joueur {
public:
    // Constructeur : crée un joueur humain avec un nom et une couleur
    JoueurHumain(const std::string& nom, Couleur couleur);

    // Implémentation de la méthode jouer pour un joueur humain
    void jouer() override;
};