#pragma once

#include "Piece.h"

class Pion : public Piece {
public:
    Pion(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;

    bool peutFairePriseEnPassant() const;
    bool peutEtrePromu() const;
};