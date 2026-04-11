#include "Cavalier.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

Cavalier::Cavalier(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

std::vector<Position> Cavalier::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    const int deplacements[8][2] = {
        {-2, -1},
        {-2, 1},
        {-1, -2},
        {-1, 2},
        {1, -2},
        {1, 2},
        {2, -1},
        {2, 1}
    };

    for (int i = 0; i < 8; ++i) {
        Position p(
            position.getLigne() + deplacements[i][0],
            position.getColonne() + deplacements[i][1]
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