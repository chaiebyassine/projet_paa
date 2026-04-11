#pragma once

#include "Piece.h"

class Tour : public Piece {
public:
    Tour(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};