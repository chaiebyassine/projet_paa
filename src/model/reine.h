#pragma once

#include "Piece.h"

class Reine : public Piece {
public:
    Reine(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};