/**
 * @file plateau.cpp
 * @brief Implémentation de la classe Plateau.
 */
#include "plateau.h"
#include "../piece/Piece.h"

// Constructeur : crée un plateau de taille lignes x colonnes
// Initialise toutes les cases vides avec leur position respective
Plateau::Plateau(int lignes, int colonnes)
    : nbLignes(lignes), nbColonnes(colonnes) {
    // Crée les cases ligne par ligne, de gauche à droite
    for (int i = 0; i < nbLignes; ++i) {
        for (int j = 0; j < nbColonnes; ++j) {
            cases.push_back(Case(Position(i, j)));
        }
    }
}

// Vérifie que la position est dans les limites du plateau
bool Plateau::estCaseValide(const Position& p) const {
    return p.getLigne() >= 0 && p.getLigne() < nbLignes &&
           p.getColonne() >= 0 && p.getColonne() < nbColonnes;
}

// Retourne la case à la position donnée (version modifiable)
// Convertit la position 2D en index 1D dans le vecteur
Case* Plateau::obtenirCase(const Position& p) {
    if (!estCaseValide(p)) {
        return nullptr;  // Position invalide
    }

    int index = p.getLigne() * nbColonnes + p.getColonne();
    return &cases[index];
}

// Retourne la case à la position donnée (version constante, en lecture seule)
const Case* Plateau::obtenirCase(const Position& p) const {
    if (!estCaseValide(p)) {
        return nullptr;
    }

    int index = p.getLigne() * nbColonnes + p.getColonne();
    return &cases[index];
}

// Place une pièce sur la case à la position donnée
bool Plateau::placerPiece(const Position& p, Piece* piece) {
    Case* c = obtenirCase(p);
    if (c == nullptr) {
        return false;  // Position invalide
    }

    c->placerPiece(piece);
    return true;
}

// Déplace une pièce de la case de départ vers la case d'arrivée
bool Plateau::deplacerPiece(const Position& depart, const Position& arrivee) {
    Case* caseDepart = obtenirCase(depart);
    Case* caseArrivee = obtenirCase(arrivee);

    if (caseDepart == nullptr || caseArrivee == nullptr) {
        return false;  // Positions invalides
    }

    if (!caseDepart->estOccupee()) {
        return false;  // Pas de pièce à déplacer
    }

    // Récupère la pièce, met à jour sa position et son état
    Piece* piece = caseDepart->getPiece();
piece->setPosition(arrivee);     // Met à jour la position de la pièce
piece->setADejaBouge(true);      // Marque que la pièce a bougé
caseArrivee->placerPiece(piece); // Place la pièce sur la case d'arrivée
caseDepart->vider();             // Vide la case de départ

    return true;
}

// Retourne le nombre de lignes du plateau
int Plateau::getNbLignes() const {
    return nbLignes;
}

// Retourne le nombre de colonnes du plateau
int Plateau::getNbColonnes() const {
    return nbColonnes;
}