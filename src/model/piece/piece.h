#pragma once

#include <vector>
#include "../base/Position.h"
#include "../base/Couleur.h"

class Plateau;
class Joueur;

// Classe mère de toutes les pièces (Roi, Reine, Tour, Fou, Cavalier, Pion)
// Chaque pièce connaît sa position, sa couleur, son propriétaire et si elle a déjà bougé
class Piece {
protected:
    Position position;    // Où se trouve la pièce sur le plateau
    Couleur couleur;      // Couleur de la pièce (BLANC, NOIR ou ROUGE)
    Joueur* joueur;       // Le joueur à qui appartient cette pièce
    bool aDejaBouge;      // Vrai si la pièce a déjà été déplacée au moins une fois

public:
    // Crée une pièce à une position donnée, avec une couleur et un propriétaire
    Piece(const Position& pos, Couleur coul, Joueur* j);

    virtual ~Piece() = default;

    // Retourne la position actuelle de la pièce
    Position getPosition() const;

    // Retourne la couleur de la pièce
    Couleur getCouleur() const;

    // Retourne le joueur propriétaire
    Joueur* getJoueur() const;

    // Retourne vrai si la pièce a déjà bougé
    bool getADejaBouge() const;

    // Met à jour la position de la pièce
    void setPosition(const Position& pos);

    // Indique que la pièce a bougé (ou pas)
    void setADejaBouge(bool valeur);

    // Calcule et retourne toutes les cases où la pièce peut aller
    // Chaque sous-classe implémente ses propres règles de déplacement
    virtual std::vector<Position> mouvementsPossibles(const Plateau& plateau) const = 0;
};