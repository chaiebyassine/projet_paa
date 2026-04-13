#pragma once

#include "../base/position.h"
#include "../base/couleur.h"
class Piece; // Déclaration anticipée pour éviter l'inclusion circulaire

// Représente une case du plateau d'échecs
// Chaque case a une position et peut contenir (ou non) une pièce
class Case {
private:
    Position position;  // Position de la case sur le plateau (ligne, colonne)
    Piece* piece;       // Pointeur vers la pièce posée sur cette case (nullptr si vide)

public:
    // Constructeur : crée une case vide à la position donnée
    Case(Position pos);

    // Retourne vrai si la case contient une pièce
    bool estOccupee() const;

    // Place une pièce sur cette case
    void placerPiece(Piece* p);

    // Retire la pièce de cette case (la case devient vide)
    void vider();

    // Retourne la pièce présente sur la case (nullptr si vide)
    Piece* getPiece() const;

    // Retourne la position de la case
    Position getPosition() const;

    // Vérifie si la case contient une pièce de couleur différente (pièce adverse)
    bool contientPieceAdverse(Couleur couleur) const;
};