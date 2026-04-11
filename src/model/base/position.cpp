#include "position.h"

Position::Position(int l, int c) {
    ligne = l;
    colonne = c;
}

int Position::getLigne() const {
    return ligne;
}

int Position::getColonne() const {
    return colonne;
}

bool Position::estValide() const {
    return ligne >= 0 && colonne >= 0;
}