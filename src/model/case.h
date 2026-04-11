#pragma once

#include "position.h"
#include "Couleur.h"
class Piece; // déclaration anticipée

class Case {
private:
    Position position;
    Piece* piece; // pointeur vers une pièce 

public:
    Case(Position pos);

    bool estOccupee() const;

    void placerPiece(Piece* p);
    void vider();

    Piece* getPiece() const;
    Position getPosition() const;
    bool contientPieceAdverse(Couleur couleur) const;
};