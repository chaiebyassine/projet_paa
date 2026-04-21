#pragma once

#include "Piece.h"

// Le Roi : pièce la plus importante, se déplace d'une seule case dans n'importe quelle direction
// Si le roi est menacé et ne peut pas s'en sortir, le joueur est éliminé (échec et mat)
class Roi : public Piece {
public:
    // Crée un roi à une position donnée
    Roi(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où le roi peut aller (1 case dans chacune des 8 directions)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};