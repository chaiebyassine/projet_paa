#pragma once

#include <vector>
#include "case.h"
#include "position.h"

class Plateau {
private:
    std::vector<Case> cases;
    int nbLignes;
    int nbColonnes;

public:
    Plateau(int lignes, int colonnes);

    bool estCaseValide(const Position& p) const;
    Case* obtenirCase(const Position& p);
    const Case* obtenirCase(const Position& p) const;

    bool placerPiece(const Position& p, Piece* piece);
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    int getNbLignes() const;
    int getNbColonnes() const;
};