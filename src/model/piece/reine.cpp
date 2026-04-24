#include "Reine.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"

Reine::Reine(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Reine : 8 directions (4 orthogonales + 4 diagonales)
std::vector<Position> Reine::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    // Orthogonales
    MoveHelper::recursiveMove(plateau, x, y, -1,  0, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1,  0, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  0, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  0,  1, couleur, moves);
    // Diagonales
    MoveHelper::recursiveMove(plateau, x, y, -1, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y, -1,  1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1, -1, couleur, moves);
    MoveHelper::recursiveMove(plateau, x, y,  1,  1, couleur, moves);
    return moves;
}
