#include "Tour.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise la tour via la classe Piece
Tour::Tour(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles de la tour
// La tour se déplace en ligne droite dans 4 directions (haut, bas, gauche, droite)
std::vector<Position> Tour::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // Les 4 directions rectilignes
    const int directions[4][2] = {
        {-1, 0}, // haut
        {1, 0},  // bas
        {0, -1}, // gauche
        {0, 1}   // droite
    };

    // Pour chaque direction, avance case par case
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

            // Avance dans la même direction
            ligne += dl;
            colonne += dc;
        }
    }

    return mouvements;
}