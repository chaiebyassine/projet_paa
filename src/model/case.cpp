#include "Case.h"

Case::Case(Position pos) : position(pos), piece(nullptr) {}

bool Case::estOccupee() const {
    return piece != nullptr;
}

void Case::placerPiece(Piece* p) {
    piece = p;
}

void Case::vider() {
    piece = nullptr;
}

Piece* Case::getPiece() const {
    return piece;
}

Position Case::getPosition() const {
    return position;
}