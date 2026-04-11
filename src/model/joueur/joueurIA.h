#pragma once

#include "Joueur.h"

class JoueurIA : public Joueur {
private:
    int profondeurRecherche;

public:
    JoueurIA(const std::string& nom, Couleur couleur, int profondeur);

    void jouer() override;
};