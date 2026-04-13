#pragma once

#include "Piece.h"

// La Reine : pièce la plus puissante, combine les mouvements de la Tour et du Fou
class Reine : public Piece {
public:
    // Constructeur : crée une reine à une position donnée
    Reine(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles (8 directions, distance illimitée)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};