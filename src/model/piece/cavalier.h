#pragma once

#include "Piece.h"

// Le Cavalier : se déplace en "L" (2 cases + 1 case perpendiculaire)
// C'est la seule pièce qui peut sauter par-dessus les autres pièces
class Cavalier : public Piece {
public:
    // Crée un cavalier à une position donnée
    Cavalier(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où le cavalier peut aller (jusqu'à 8 cases en L)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};