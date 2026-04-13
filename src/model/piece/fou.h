#pragma once

#include "Piece.h"

// Le Fou : se déplace en diagonale sans limite de distance
class Fou : public Piece {
public:
    // Constructeur : crée un fou à une position donnée
    Fou(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles (4 diagonales)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};