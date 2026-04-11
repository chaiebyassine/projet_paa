#include "Pion.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

Pion::Pion(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

std::vector<Position> Pion::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    int direction = 0;

    if (couleur == Couleur::BLANC) {
        direction = -1;
    } else {
        direction = 1;
    }

    // Avancer d'une case
    Position avant(position.getLigne() + direction, position.getColonne());
    if (plateau.estCaseValide(avant)) {
        const Case* caseAvant = plateau.obtenirCase(avant);
        if (caseAvant != nullptr && !caseAvant->estOccupee()) {
            mouvements.push_back(avant);

            // Avancer de deux cases au premier coup
            if (!aDejaBouge) {
                Position avantDeux(position.getLigne() + 2 * direction, position.getColonne());
                if (plateau.estCaseValide(avantDeux)) {
                    const Case* caseAvantDeux = plateau.obtenirCase(avantDeux);
                    if (caseAvantDeux != nullptr && !caseAvantDeux->estOccupee()) {
                        mouvements.push_back(avantDeux);
                    }
                }
            }
        }
    }

    // Capture diagonale gauche
    Position diagGauche(position.getLigne() + direction, position.getColonne() - 1);
    if (plateau.estCaseValide(diagGauche)) {
        const Case* caseDiagGauche = plateau.obtenirCase(diagGauche);
        if (caseDiagGauche != nullptr && caseDiagGauche->contientPieceAdverse(couleur)) {
            mouvements.push_back(diagGauche);
        }
    }

    // Capture diagonale droite
    Position diagDroite(position.getLigne() + direction, position.getColonne() + 1);
    if (plateau.estCaseValide(diagDroite)) {
        const Case* caseDiagDroite = plateau.obtenirCase(diagDroite);
        if (caseDiagDroite != nullptr && caseDiagDroite->contientPieceAdverse(couleur)) {
            mouvements.push_back(diagDroite);
        }
    }

    return mouvements;
}

bool Pion::peutFairePriseEnPassant() const {
    return false;
}

bool Pion::peutEtrePromu() const {
    if (couleur == Couleur::BLANC) {
        return position.getLigne() == 0;
    }
    return position.getLigne() == 7;
}