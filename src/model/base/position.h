#pragma once

// Représente une position sur le plateau avec une ligne et une colonne
class Position {
private:
    int ligne;    // Numéro de ligne (0 = haut du plateau)
    int colonne;  // Numéro de colonne (0 = gauche du plateau)

public:
    // Constructeur : crée une position à la ligne l et colonne c
    Position(int l, int c);

    // Retourne le numéro de ligne
    int getLigne() const;

    // Retourne le numéro de colonne
    int getColonne() const;

    // Vérifie si la position est valide (coordonnées positives)
    bool estValide() const;
};