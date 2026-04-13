#include "position.h"

// Constructeur : initialise la ligne et la colonne
Position::Position(int l, int c) {
    ligne = l;
    colonne = c;
}

// Retourne la ligne de la position
int Position::getLigne() const {
    return ligne;
}

// Retourne la colonne de la position
int Position::getColonne() const {
    return colonne;
}

// Vérifie que les coordonnées sont positives ou nulles
bool Position::estValide() const {
    return ligne >= 0 && colonne >= 0;
}