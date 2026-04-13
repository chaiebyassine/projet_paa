#pragma once

#include <vector>
#include "../base/Position.h"
#include "../base/Couleur.h"

class Plateau;  // Déclaration anticipée du plateau
class Joueur;   // Déclaration anticipée du joueur

// Classe abstraite représentant une pièce d'échecs
// Classe de base pour Roi, Reine, Tour, Fou, Cavalier et Pion
class Piece {
protected:
    Position position;    // Position actuelle de la pièce sur le plateau
    Couleur couleur;      // Couleur de la pièce (BLANC, NOIR ou ROUGE)
    Joueur* joueur;       // Pointeur vers le joueur propriétaire de cette pièce
    bool aDejaBouge;      // Indique si la pièce a déjà été déplacée (utile pour le roque, le pion, etc.)

public:
    // Constructeur : crée une pièce à une position, avec une couleur et un propriétaire
    Piece(const Position& pos, Couleur coul, Joueur* j);

    // Destructeur virtuel pour permettre la destruction correcte des sous-classes
    virtual ~Piece() = default;

    // Retourne la position actuelle de la pièce
    Position getPosition() const;

    // Retourne la couleur de la pièce
    Couleur getCouleur() const;

    // Retourne le joueur propriétaire de la pièce
    Joueur* getJoueur() const;

    // Retourne vrai si la pièce a déjà été déplacée
    bool getADejaBouge() const;

    // Modifie la position de la pièce
    void setPosition(const Position& pos);

    // Modifie l'état "a déjà bougé"
    void setADejaBouge(bool valeur);

    // Méthode abstraite : retourne la liste des positions où la pièce peut se déplacer
    virtual std::vector<Position> mouvementsPossibles(const Plateau& plateau) const = 0;
};