#pragma once

#include "Piece.h"

// Le Pion : avance d'une case vers l'avant, capture en diagonale
// Au premier coup il peut avancer de 2 cases, et il peut être promu en arrivant au bout
class Pion : public Piece {
public:
    // Crée un pion à une position donnée
    Pion(const Position& pos, Couleur coul, Joueur* j);

    // Retourne toutes les cases où le pion peut aller (avancer + captures diagonales)
    std::vector<Position> mouvementsPossibles(const Plateau& plateau) const override;

    // Retourne vrai si le pion peut faire une prise en passant
    bool peutFairePriseEnPassant() const;

    // Retourne vrai si le pion a atteint la dernière rangée et peut être promu
    bool peutEtrePromu() const;
};