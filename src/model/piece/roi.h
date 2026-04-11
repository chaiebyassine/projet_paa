#pragma once

#include "Piece.h"

class Roi : public Piece {
public:
    Roi(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};