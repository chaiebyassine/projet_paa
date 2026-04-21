/**
 * @file reine.cpp
 * @brief Implémentation de la classe Reine.
 */
#include "Reine.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise la reine via la classe Piece
Reine::Reine(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles de la reine
// La reine se déplace dans les 8 directions sans limite de distance
std::vector<Position> Reine::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // Les 8 directions : horizontales, verticales et diagonales
    const int directions[8][2] = {
        {-1, 0},  // haut
        {1, 0},   // bas
        {0, -1},  // gauche
        {0, 1},   // droite
        {-1, -1}, // haut-gauche (diagonale)
        {-1, 1},  // haut-droite (diagonale)
        {1, -1},  // bas-gauche (diagonale)
        {1, 1}    // bas-droite (diagonale)
    };

    // Pour chaque direction, avance case par case
    for (int d = 0; d < 8; ++d) {
        int dl = directions[d][0];  // Déplacement en ligne
        int dc = directions[d][1];  // Déplacement en colonne

        int ligne = position.getLigne() + dl;
        int colonne = position.getColonne() + dc;

        // Continue tant qu'on est sur le plateau
        while (true) {
            Position p(ligne, colonne);

            if (!plateau.estCaseValide(p)) {
                break;  // Hors du plateau, on arrête dans cette direction
            }

            const Case* c = plateau.obtenirCase(p);
            if (c == nullptr) {
                break;
            }

            if (!c->estOccupee()) {
                // Case vide : mouvement possible, on continue
                mouvements.push_back(p);
            } else {
                // Case occupée : capture possible si pièce adverse, puis on arrête
                if (c->contientPieceAdverse(couleur)) {
                    mouvements.push_back(p);
                }
                break;  // Bloqué par une pièce (alliée ou adverse)
            }

            // Avance d'une case dans la même direction
            ligne += dl;
            colonne += dc;
        }
    }

    return mouvements;
}