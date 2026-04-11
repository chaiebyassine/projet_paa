#pragma once

#include "Joueur.h"

class JoueurHumain : public Joueur {
public:
    JoueurHumain(const std::string& nom, Couleur couleur);

    void jouer() override;
};