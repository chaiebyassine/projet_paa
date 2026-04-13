#pragma once

#include "Piece.h"

// Le Cavalier : se déplace en "L" (2 cases + 1 case perpendiculaire), peut sauter par-dessus les pièces
class Cavalier : public Piece {
public:
    // Constructeur : crée un cavalier à une position donnée
    Cavalier(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles (jusqu'à 8 cases en L)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};