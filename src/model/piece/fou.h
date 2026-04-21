#pragma once

#include "Piece.h"

// Le Fou : se déplace uniquement en diagonale, sans limite de cases
// Il reste toujours sur les cases de la même couleur tout au long de la partie
class Fou : public Piece {
public:
    // Crée un fou à une position donnée
    Fou(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où le fou peut aller (4 diagonales)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;

    std::string getSymbole() const override { return "F"; }
};