#include "Fou.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"

Fou::Fou(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Fou : 4 directions diagonales (logique 2D style projetexp)
std::vector<Position> Fou::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    MoveHelper::recursiveMove(plateau, x, y, -1, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y, -1,  1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1,  1, couleur, moves);
    return moves;
}
