#include "Case.h"
#include "../piece/Piece.h"

Case::Case(const Position& pos) : position(pos), piece(nullptr) {}

const Position& Case::getPosition() const { return position; }

Piece* Case::getPiece()    const { return piece; }
Piece* Case::obtenirPiece() const { return piece; }

void Case::setPiece(Piece* p)    { piece = p; }
void Case::placerPiece(Piece* p) { piece = p; if (p) p->setPosition(this->position); }
void Case::vider()               { piece = nullptr; }

bool Case::estOccupee() const { return piece != nullptr; }

bool Case::contientPieceAdverse(Couleur c) const {
    return piece != nullptr && piece->getCouleur() != c;
}

void Case::ajouterVoisin(const Position& pos) {
    voisins.push_back(pos);
}

const std::vector<Position>& Case::getVoisins() const { return voisins; }
