#pragma once

// Représente une case du plateau par ses coordonnées (ligne, colonne)
class Position {
private:
    int ligne;    // Numéro de ligne (0 = haut du plateau)
    int colonne;  // Numéro de colonne (0 = gauche du plateau)

public:
    // Crée une position aux coordonnées (l, c)
    Position(int l, int c);

    // Retourne le numéro de ligne
    int getLigne() const;

    // Retourne le numéro de colonne
    int getColonne() const;

    // Retourne vrai si les deux coordonnées sont >= 0
    bool estValide() const;
};