#pragma once

#include "Piece.h"

// Le Pion : se déplace d'une case vers l'avant, capture en diagonale
// Peut avancer de 2 cases au premier coup
class Pion : public Piece {
public:
    // Constructeur : crée un pion à une position donnée
    Pion(const Position& pos, Couleur coul, Joueur* j);

    // Retourne la liste des mouvements possibles (avancer + captures diagonales)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;

    // Vérifie si le pion peut faire une prise en passant (non implémenté pour l'instant)
    bool peutFairePriseEnPassant() const;

    // Vérifie si le pion a atteint le bord opposé et peut être promu
    bool peutEtrePromu() const;
};