#pragma once

#include <vector>
#include "../base/Position.h"
#include "../base/Couleur.h"

class Plateau;
class Joueur;

class Piece {
protected:
    Position position;
    Couleur couleur;
    Joueur* joueur;
    bool aDejaBouge;

public:
    Piece(const Position& pos, Couleur coul, Joueur* j);
    virtual ~Piece() = default;

    Position getPosition() const;
    Couleur getCouleur() const;
    Joueur* getJoueur() const;
    bool getADejaBouge() const;

    void setPosition(const Position& pos);
    void setADejaBouge(bool valeur);

    virtual std::vector<Position> mouvementsPossibles(const Plateau& plateau) const = 0;
};