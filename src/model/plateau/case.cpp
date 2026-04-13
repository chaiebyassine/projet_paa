#include "Case.h"
#include "../piece/Piece.h"

// Constructeur : crée une case vide (piece = nullptr) à la position donnée
Case::Case(Position pos) : position(pos), piece(nullptr) {}

// Retourne vrai si une pièce est posée sur cette case
bool Case::estOccupee() const {
    return piece != nullptr;
}

// Place une pièce sur cette case
void Case::placerPiece(Piece* p) {
    piece = p;
}

// Vide la case en retirant la pièce
void Case::vider() {
    piece = nullptr;
}

// Retourne la pièce présente sur la case (ou nullptr si vide)
Piece* Case::getPiece() const {
    return piece;
}

// Retourne la position de cette case sur le plateau
Position Case::getPosition() const {
    return position;
}

// Vérifie si la case contient une pièce adverse (couleur différente de celle passée en paramètre)
bool Case::contientPieceAdverse(Couleur couleur) const {
    return piece != nullptr && piece->getCouleur() != couleur;
}