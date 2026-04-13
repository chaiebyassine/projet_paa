#pragma once

#include "Piece.h"

// La Tour : se déplace en ligne droite (horizontalement ou verticalement) sans limite de distance
class Tour : public Piece {
public:
    // Constructeur : crée une tour à une position donnée
    Tour(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles (4 directions rectilignes)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;
};