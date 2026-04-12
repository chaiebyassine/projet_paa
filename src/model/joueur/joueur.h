#pragma once

#include <string>
#include <vector>
#include "../base/Couleur.h"

class Piece;

class Joueur {
protected:
    std::string nom;
    Couleur couleur;
    bool estElimine;
    std::vector<Piece*> pieces;

public:
    Joueur(const std::string& nom, Couleur couleur);
    virtual ~Joueur() = default;

    std::string getNom() const;
    Couleur getCouleur() const;
    bool getEstElimine() const;
void retirerPiece(Piece* p);
    void ajouterPiece(Piece* p);
    std::vector<Piece*> getPieces() const;

    virtual void jouer() = 0; // important (abstrait)
};