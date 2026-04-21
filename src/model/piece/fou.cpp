/**
 * @file fou.cpp
 * @brief Implémentation de la classe Fou.
 */
#include "Fou.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise le fou via la classe Piece
Fou::Fou(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles du fou
// Le fou se déplace uniquement en diagonale sans limite de distance
std::vector<Position> Fou::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // Les 4 directions diagonales
    const int directions[4][2] = {
        {-1, -1}, // diagonale haut-gauche
        {-1, 1},  // diagonale haut-droite
        {1, -1},  // diagonale bas-gauche
        {1, 1}    // diagonale bas-droite
    };

    // Pour chaque diagonale, avance case par case
    for (int d = 0; d < 4; ++d) {
        int dl = directions[d][0];  // Déplacement en ligne
        int dc = directions[d][1];  // Déplacement en colonne

        int ligne = position.getLigne() + dl;
        int colonne = position.getColonne() + dc;

        // Continue tant qu'on est sur le plateau
        while (true) {
            Position p(ligne, colonne);

            if (!plateau.estCaseValide(p)) {
                break;  // Hors du plateau
            }

            const Case* c = plateau.obtenirCase(p);
            if (c == nullptr) {
                break;
            }

            if (!c->estOccupee()) {
                // Case vide : mouvement possible, on continue
                mouvements.push_back(p);
            } else {
                // Case occupée : capture possible si pièce adverse
                if (c->contientPieceAdverse(couleur)) {
                    mouvements.push_back(p);
                }
                break;  // Bloqué par une pièce
            }

            // Avance dans la même diagonale
            ligne += dl;
            colonne += dc;
        }
    }

    return mouvements;
}