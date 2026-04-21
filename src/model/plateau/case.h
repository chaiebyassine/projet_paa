#pragma once

#include "../base/position.h"
#include "../base/couleur.h"
class Piece;

// Représente une case du plateau : elle a une position et peut contenir une pièce (ou être vide)
class Case {
private:
    Position position;  // Coordonnées de la case sur le plateau
    Piece* piece;       // Pièce posée sur la case (nullptr si vide)

public:
    // Crée une case vide à la position donnée
    Case(Position pos);

    // Retourne vrai s'il y a une pièce sur la case
    bool estOccupee() const;

    // Pose une pièce sur la case
    void placerPiece(Piece* p);

    // Enleve la pièce de la case (la case devient vide)
    void vider();

    // Retourne la pièce sur la case (nullptr si vide)
    Piece* getPiece() const;

    // Retourne la position de la case
    Position getPosition() const;

    // Retourne vrai si la case contient une pièce qui n'est pas de la même couleur
    bool contientPieceAdverse(Couleur couleur) const;
};