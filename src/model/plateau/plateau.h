#pragma once

#include <vector>
#include "case.h"
#include "../base/position.h"

// Le plateau de jeu : une grille de cases qui stocke toutes les pièces
class Plateau {
private:
    std::vector<Case> cases;  // Toutes les cases du plateau stockées dans un tableau
    int nbLignes;             // Nombre de lignes
    int nbColonnes;           // Nombre de colonnes

public:
    // Crée un plateau vide de taille lignes x colonnes
    Plateau(int lignes, int colonnes);

    // Retourne vrai si la position est dans les limites du plateau
    bool estCaseValide(const Position& p) const;

    // Retourne la case à cette position (version modifiable)
    Case* obtenirCase(const Position& p);

    // Retourne la case à cette position (version lecture seule)
    const Case* obtenirCase(const Position& p) const;

    // Place une pièce sur la case à la position donnée
    bool placerPiece(const Position& p, Piece* piece);

    // Déplace la pièce de la case de départ vers la case d'arrivée
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    // Retourne le nombre de lignes
    int getNbLignes() const;

    // Retourne le nombre de colonnes
    int getNbColonnes() const;
};