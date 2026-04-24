#include "Cavalier.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"
#include <algorithm>

Cavalier::Cavalier(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

// Cavalier : 8 sauts en L (2 cases ortho + 1 case perpendiculaire)
std::vector<Position> Cavalier::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;

    // 2 en x + 1 en y
    MoveHelper::knightHop(plateau, x, y,  1, 0, 0,  1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y,  1, 0, 0, -1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, -1, 0, 0,  1, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, -1, 0, 0, -1, couleur, moves);
    // 2 en y + 1 en x
    MoveHelper::knightHop(plateau, x, y, 0,  1,  1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0,  1, -1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0, -1,  1, 0, couleur, moves);
    MoveHelper::knightHop(plateau, x, y, 0, -1, -1, 0, couleur, moves);

    // Dédoublonnage
    std::sort(moves.begin(), moves.end());
    moves.erase(std::unique(moves.begin(), moves.end()), moves.end());
    return moves;
}
