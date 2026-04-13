#pragma once

#include <vector>
#include "case.h"
#include "../base/position.h"

// Représente le plateau de jeu d'échecs
// Contient une grille de cases et gère le placement/déplacement des pièces
class Plateau {
private:
    std::vector<Case> cases;  // Tableau linéaire contenant toutes les cases du plateau
    int nbLignes;             // Nombre de lignes du plateau
    int nbColonnes;           // Nombre de colonnes du plateau

public:
    // Constructeur : crée un plateau vide de taille lignes x colonnes
    Plateau(int lignes, int colonnes);

    // Vérifie si une position est dans les limites du plateau
    bool estCaseValide(const Position& p) const;

    // Retourne un pointeur vers la case à la position donnée (version modifiable)
    Case* obtenirCase(const Position& p);

    // Retourne un pointeur vers la case à la position donnée (version constante)
    const Case* obtenirCase(const Position& p) const;

    // Place une pièce à la position donnée sur le plateau
    bool placerPiece(const Position& p, Piece* piece);

    // Déplace une pièce de la position de départ vers la position d'arrivée
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    // Retourne le nombre de lignes du plateau
    int getNbLignes() const;

    // Retourne le nombre de colonnes du plateau
    int getNbColonnes() const;
};