#include "Piece.h"

Piece::Piece(const Position& pos, Couleur coul, Joueur* j)
    : position(pos), couleur(coul), joueur(j), aDejaBouge(false) {
}

Position Piece::getPosition() const {
    return position;
}

Couleur Piece::getCouleur() const {
    return couleur;
}

Joueur* Piece::getJoueur() const {
    return joueur;
}

bool Piece::getADejaBouge() const {
    return aDejaBouge;
}

void Piece::setPosition(const Position& pos) {
    position = pos;
}

void Piece::setADejaBouge(bool valeur) {
    aDejaBouge = valeur;
}