#pragma once

#include "Joueur.h"

// Joueur contrôlé par l'ordinateur via l'algorithme minimax/alpha-beta
class JoueurIA : public Joueur {
private:
    int profondeurRecherche;  // Plus la profondeur est grande, plus l'IA réfléchit loin

public:
    // Crée un joueur IA avec un nom, une couleur et une profondeur de calcul
    JoueurIA(const std::string& nom, Couleur couleur, int profondeur);

    // L'IA calcule le meilleur coup et le joue
    void jouer() override;
};