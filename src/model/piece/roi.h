#pragma once

#include "Piece.h"

// Le Roi : pièce la plus importante, se déplace d'une case dans toutes les directions
class Roi : public Piece {
public:
    // Constructeur : crée un roi à une position donnée
    Roi(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles du roi (1 case dans les 8 directions)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};