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

    // Détermine la direction d'avancement selon la couleur
    // BLANC avance vers le haut (-1), les autres vers le bas (+1)
    int direction = 0;

    if (couleur == Couleur::BLANC) {
        direction = -1;  // Les blancs montent sur le plateau
    } else {
        direction = 1;   // Les noirs/rouges descendent
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
    if (couleur == Couleur::BLANC) {
        return position.getLigne() == 0;  // Les blancs sont promus en haut
    }
    return position.getLigne() == 7;      // Les autres sont promus en bas
}