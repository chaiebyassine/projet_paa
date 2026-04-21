/**
 * @file pion.cpp
 * @brief Implémentation de la classe Pion.
 */
#include "Pion.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise le pion via la classe Piece
Pion::Pion(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles du pion
// Le pion avance d'une case, de 2 cases au 1er coup, et capture en diagonale
std::vector<Position> Pion::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // NOIR avance vers les lignes décroissantes (de la ligne 10 vers la ligne 0)
    // BLANC et ROUGE avancent vers les lignes croissantes (vers la ligne 11)
    int direction = 0;

    if (couleur == Couleur::NOIR) {
        direction = -1;
    } else {
        direction = 1;
    }

    // --- Avancer d'une case ---
    Position avant(position.getLigne() + direction, position.getColonne());
    if (plateau.estCaseValide(avant)) {
        const Case* caseAvant = plateau.obtenirCase(avant);
        if (caseAvant != nullptr && !caseAvant->estOccupee()) {
            mouvements.push_back(avant);

            // --- Avancer de deux cases au premier coup ---
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

    // --- Capture diagonale gauche ---
    Position diagGauche(position.getLigne() + direction, position.getColonne() - 1);
    if (plateau.estCaseValide(diagGauche)) {
        const Case* caseDiagGauche = plateau.obtenirCase(diagGauche);
        if (caseDiagGauche != nullptr && caseDiagGauche->contientPieceAdverse(couleur)) {
            mouvements.push_back(diagGauche);
        }
    }

    // --- Capture diagonale droite ---
    Position diagDroite(position.getLigne() + direction, position.getColonne() + 1);
    if (plateau.estCaseValide(diagDroite)) {
        const Case* caseDiagDroite = plateau.obtenirCase(diagDroite);
        if (caseDiagDroite != nullptr && caseDiagDroite->contientPieceAdverse(couleur)) {
            mouvements.push_back(diagDroite);
        }
    }

    return mouvements;
}

// Prise en passant : non implémentée pour l'instant
bool Pion::peutFairePriseEnPassant() const {
    return false;
}

// Vérifie si le pion peut être promu (arrivé au bout du plateau)
bool Pion::peutEtrePromu() const {
    // NOIR est promu quand il atteint la ligne 0 (zone Blanc)
    if (couleur == Couleur::NOIR) {
        return position.getLigne() == 0;
    }
    // BLANC et ROUGE sont promus quand ils atteignent la ligne 11 (zone Noir)
    return position.getLigne() == 11;
    // BLANC et ROUGE sont promus quand ils atteignent la ligne 11 (zone Noir)
    return position.getLigne() == 11;
}