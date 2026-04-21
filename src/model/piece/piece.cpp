/**
 * @file piece.cpp
 * @brief Implémentation des méthodes de la classe Piece.
 */
#include "Piece.h"

// Constructeur : initialise la position, la couleur, le joueur, et la pièce n'a pas encore bougé
Piece::Piece(const Position& pos, Couleur coul, Joueur* j)
    : position(pos), couleur(coul), joueur(j), aDejaBouge(false) {
}

// Retourne la position actuelle de la pièce
Position Piece::getPosition() const {
    return position;
}

// Retourne la couleur de la pièce
Couleur Piece::getCouleur() const {
    return couleur;
}

// Retourne le joueur propriétaire
Joueur* Piece::getJoueur() const {
    return joueur;
}

// Retourne vrai si la pièce a déjà été déplacée au moins une fois
bool Piece::getADejaBouge() const {
    return aDejaBouge;
}

// Met à jour la position de la pièce
void Piece::setPosition(const Position& pos) {
    position = pos;
}

// Met à jour l'état "a déjà bougé" de la pièce
void Piece::setADejaBouge(bool valeur) {
    aDejaBouge = valeur;
}