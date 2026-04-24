#include "Pion.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../base/moveHelper.h"

Pion::Pion(const Position& pos, Couleur coul, Joueur* j) : Piece(pos, coul, j) {}

std::vector<Position> Pion::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> moves;
    auto [x, y] = positionTo2D(position);
    if (x < 0) return moves;
    MoveHelper::pawnStep(plateau, x, y, couleur, aDejaBouge, moves);
    return moves;
}

bool Pion::peutFairePriseEnPassant() const { return false; }

bool Pion::peutEtrePromu() const {
    // Un pion est promu lorsqu'il atteint la rangée arrière d'un adversaire
    if (couleur == Couleur::BLANC) return position.q == 4 || position.r == 4;
    if (couleur == Couleur::ROUGE) return position.r == 4 || position.s == 4;
    return position.s == 4 || position.q == 4;
}
