#pragma once
#include <vector>
#include "../base/Position.h"
#include "../base/Couleur.h"

class Piece;

class Case {
private:
    Position position;
    Piece* piece;
    std::vector<Position> voisins;

public:
    Case(const Position& pos);

    const Position& getPosition() const;

    // Accès à la pièce
    Piece* getPiece() const;
    Piece* obtenirPiece() const;   // alias

    void setPiece(Piece* p);
    void placerPiece(Piece* p);    // alias
    void vider();                  // retire la pièce (= setPiece(nullptr))

    bool estOccupee() const;

    // Retourne vrai si la case contient une pièce adverse (couleur différente)
    bool contientPieceAdverse(Couleur c) const;

    void ajouterVoisin(const Position& pos);
    const std::vector<Position>& getVoisins() const;
};
