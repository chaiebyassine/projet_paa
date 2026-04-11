#pragma once

#include "Piece.h"

class Cavalier : public Piece {
public:
    Cavalier(const Position& pos, Couleur coul, Joueur* j);

    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};