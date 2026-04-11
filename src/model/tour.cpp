#include "Tour.h"
#include "Plateau.h"

Tour::Tour(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

std::vector<Position> Tour::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    Position haut(position.getLigne() - 1, position.getColonne());
    Position bas(position.getLigne() + 1, position.getColonne());
    Position gauche(position.getLigne(), position.getColonne() - 1);
    Position droite(position.getLigne(), position.getColonne() + 1);

    if (plateau.estCaseValide(haut)) mouvements.push_back(haut);
    if (plateau.estCaseValide(bas)) mouvements.push_back(bas);
    if (plateau.estCaseValide(gauche)) mouvements.push_back(gauche);
    if (plateau.estCaseValide(droite)) mouvements.push_back(droite);

    return mouvements;
}