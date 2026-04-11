#pragma once

#include "Piece.h"

class Fou : public Piece {
public:
    Fou(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};