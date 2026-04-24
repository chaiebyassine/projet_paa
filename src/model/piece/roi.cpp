#include "Roi.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"

Roi::Roi(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Roi : 1 case dans les 8 directions
std::vector<Position> Roi::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    MoveHelper::oneStep(plateau, x, y, -1,  0, couleur, moves);
    MoveHelper::oneStep(plateau, x, y,  1,  0, couleur, moves);
    MoveHelper::oneStep(plateau, x, y,  0, -1, couleur, moves);
    MoveHelper::oneStep(plateau, x, y,  0,  1, couleur, moves);
    MoveHelper::oneStep(plateau, x, y, -1, -1, couleur, moves);
    MoveHelper::oneStep(plateau, x, y, -1,  1, couleur, moves);
    MoveHelper::oneStep(plateau, x, y,  1, -1, couleur, moves);
    MoveHelper::oneStep(plateau, x, y,  1,  1, couleur, moves);
    return moves;
}
