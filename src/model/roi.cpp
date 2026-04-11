#include "Roi.h"
#include "Plateau.h"
#include "Case.h"
#include "Piece.h"

Roi::Roi(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

std::vector<Position> Roi::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    const int directions[8][2] = {
        {-1, -1},
        {-1, 0},
        {-1, 1},
        {0, -1},
        {0, 1},
        {1, -1},
        {1, 0},
        {1, 1}
    };

    for (int i = 0; i < 8; ++i) {
        Position p(
            position.getLigne() + directions[i][0],
            position.getColonne() + directions[i][1]
        );

        if (!plateau.estCaseValide(p)) {
            continue;
        }

        const Case* c = plateau.obtenirCase(p);
        if (c == nullptr) {
            continue;
        }

        if (!c->estOccupee() || c->contientPieceAdverse(couleur)) {
            mouvements.push_back(p);
        }
    }

    return mouvements;
}