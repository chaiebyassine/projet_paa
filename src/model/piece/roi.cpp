#include "Roi.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise le roi via la classe Piece
Roi::Roi(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles du roi
// Le roi peut se déplacer d'une seule case dans les 8 directions
std::vector<Position> Roi::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // Les 8 directions possibles : diagonales, horizontales et verticales
    const int directions[8][2] = {
        {-1, -1},  // haut-gauche
        {-1, 0},   // haut
        {-1, 1},   // haut-droite
        {0, -1},   // gauche
        {0, 1},    // droite
        {1, -1},   // bas-gauche
        {1, 0},    // bas
        {1, 1}     // bas-droite
    };

    // Vérifie chaque direction
    for (int i = 0; i < 8; ++i) {
        Position p(
            position.getLigne() + directions[i][0],
            position.getColonne() + directions[i][1]
        );

        // Vérifie que la case est dans les limites du plateau
        if (!plateau.estCaseValide(p)) {
            continue;
        }

        const Case* c = plateau.obtenirCase(p);
        if (c == nullptr) {
            continue;
        }

        // Le roi peut aller sur une case vide ou capturer une pièce adverse
        if (!c->estOccupee() || c->contientPieceAdverse(couleur)) {
            mouvements.push_back(p);
        }
    }

    return mouvements;
}