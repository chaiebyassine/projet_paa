#include "Cavalier.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "Piece.h"

// Constructeur : initialise le cavalier via la classe Piece
Cavalier::Cavalier(const Position& pos, Couleur coul, Joueur* j)
    : Piece(pos, coul, j) {
}

// Calcule tous les mouvements possibles du cavalier
// Le cavalier se déplace en "L" : 2 cases dans une direction + 1 case perpendiculaire
// C'est la seule pièce qui peut sauter par-dessus d'autres pièces
std::vector<Position> Cavalier::mouvementsPossibles(const Plateau& plateau) const {
    std::vector<Position> mouvements;

    // Les 8 déplacements possibles en L
    const int deplacements[8][2] = {
        {-2, -1},  // 2 haut, 1 gauche
        {-2, 1},   // 2 haut, 1 droite
        {-1, -2},  // 1 haut, 2 gauche
        {-1, 2},   // 1 haut, 2 droite
        {1, -2},   // 1 bas, 2 gauche
        {1, 2},    // 1 bas, 2 droite
        {2, -1},   // 2 bas, 1 gauche
        {2, 1}     // 2 bas, 1 droite
    };

    // Vérifie chaque déplacement en L
    for (int i = 0; i < 8; ++i) {
        Position p(
            position.getLigne() + deplacements[i][0],
            position.getColonne() + deplacements[i][1]
        );

        // Vérifie que la case est dans les limites du plateau
        if (!plateau.estCaseValide(p)) {
            continue;
        }

        const Case* c = plateau.obtenirCase(p);
        if (c == nullptr) {
            continue;
        }

        // Le cavalier peut aller sur une case vide ou capturer une pièce adverse
        if (!c->estOccupee() || c->contientPieceAdverse(couleur)) {
            mouvements.push_back(p);
        }
    }

    return mouvements;
}