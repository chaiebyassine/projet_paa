#include "Tour.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

Tour::Tour(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

std::vector<Position> Tour::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    const int directions[4][2] = {
        {-1, 0}, // haut
        {1, 0},  // bas
        {0, -1}, // gauche
        {0, 1}   // droite
    };

    for (int d = 0; d < 4; ++d) {
        int dl = directions[d][0];
        int dc = directions[d][1];

        int ligne = position.getLigne() + dl;
        int colonne = position.getColonne() + dc;

        while (true) {
            Position p(ligne, colonne);

            if (!plateau.estCaseValide(p)) {
                break;
            }

            const Case* c = plateau.obtenirCase(p);
            if (c == nullptr) {
                break;
            }

            if (!c->estOccupee()) {
                mouvements.push_back(p);
            } else {
                if (c->contientPieceAdverse(couleur)) {
                    mouvements.push_back(p);
                }
                break;
            }

            ligne += dl;
            colonne += dc;
        }
    }

    return mouvements;
}