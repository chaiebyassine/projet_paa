#pragma once

#include "Piece.h"

// La Tour : se déplace en ligne droite (horizontale ou verticale) sans limite de cases
// Elle est bloquée si une autre pièce se trouve sur sa trajectoire
class Tour : public Piece {
public:
    // Crée une tour à une position donnée
    Tour(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où la tour peut aller (4 directions rectilignes)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;

    std::string getSymbole() const override { return "T"; }
};