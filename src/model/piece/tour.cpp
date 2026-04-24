#include "Tour.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"

Tour::Tour(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Tour : 4 directions orthogonales (logique 2D style projetexp)
std::vector<Position> Tour::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    MoveHelper::recursiveMove(plateau, x, y, -1,  0, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1,  0, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  0, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  0,  1, couleur, moves);
    return moves;
}
