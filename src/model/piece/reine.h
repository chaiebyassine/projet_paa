#pragma once

#include "Piece.h"

// La Reine : pièce la plus puissante
// Combine les mouvements de la Tour (lignes droites) et du Fou (diagonales)
class Reine : public Piece {
public:
    // Crée une reine à une position donnée
    Reine(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où la reine peut aller (8 directions sans limite)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};