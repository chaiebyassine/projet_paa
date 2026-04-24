#pragma once
#include "Position.h"
#include <utility>

// Conversion Position cubique ↔ (x, y) 12×12 grille Yalta (style projetexp).
// Grille 12x12 avec 6 matrices 4x4 :
//   mat1 (haut gauche)  : x<4,  y<4     — BLANC mat A
//   mat2 (haut milieu)  : x<4,  y∈[4,7] — BLANC mat B
//   mat3 (milieu gauche): y<4,  x∈[4,7] — ROUGE mat B
//   mat4 (milieu droite): x∈[4,7], y∈[8,11] — ROUGE mat A
//   mat5 (bas milieu)   : x∈[8,11], y∈[4,7] — NOIR  mat A
//   mat6 (bas droite)   : x∈[8,11], y∈[8,11] — NOIR  mat B

inline std::pair<int,int> positionTo2D(const Position& p) {
    // BLANC : s∈[1,4], r∈[-7,0]
    if (p.s >= 1 && p.s <= 4 && p.r >= -7 && p.r <= 0) {
        return { 4 - p.s, -p.r };
    }
    // ROUGE : q∈[1,4], s∈[-7,0]
    if (p.q >= 1 && p.q <= 4 && p.s >= -7 && p.s <= 0) {
        int x = 3 + p.q;
        int y = (-p.s < 4) ? (11 + p.s) : (7 + p.s);
        return { x, y };
    }
    // NOIR : r∈[1,4], q∈[-7,0]
    if (p.r >= 1 && p.r <= 4 && p.q >= -7 && p.q <= 0) {
        int x = 7 + p.r;
        int y = (-p.q < 4) ? (7 + p.q) : (4 - p.q);
        return { x, y };
    }
    return { -1, -1 };
}

inline Position positionFrom2D(int x, int y) {
    // BLANC
    if (x >= 0 && x < 4 && y >= 0 && y < 8) {
        int s = 4 - x, r = -y, q = -r - s;
        return Position(q, r, s);
    }
    // ROUGE mat=3 (y<4, x∈[4,7])
    if (x >= 4 && x < 8 && y >= 0 && y < 4) {
        int q = x - 3, s = y - 7, r = -q - s;
        return Position(q, r, s);
    }
    // ROUGE mat=4 (y∈[8,11], x∈[4,7])
    if (x >= 4 && x < 8 && y >= 8 && y < 12) {
        int q = x - 3, s = y - 11, r = -q - s;
        return Position(q, r, s);
    }
    // NOIR mat=5 (x∈[8,11], y∈[4,7])
    if (x >= 8 && x < 12 && y >= 4 && y < 8) {
        int r = x - 7, q = y - 7, s = -q - r;
        return Position(q, r, s);
    }
    // NOIR mat=6 (x∈[8,11], y∈[8,11])
    if (x >= 8 && x < 12 && y >= 8 && y < 12) {
        int r = x - 7, q = 4 - y, s = -q - r;
        return Position(q, r, s);
    }
    return INVALID_POS();
}

// Sous-matrice (1..6) d'une case (x, y) — utilisé par adjustCoordinates
inline int determineSubMatrix2D(int x, int y) {
    if (x < 4  && y < 4)                     return 1;
    if (x < 4  && y >= 4 && y < 8)           return 2;
    if (y < 4  && x >= 4 && x < 8)           return 3;
    if (y >= 7 && x >= 4 && x < 8)           return 4;
    if (x >= 7 && y >= 4 && y < 8)           return 5;
    if (x >= 7 && y >= 7)                    return 6;
    return 0;
}

// Vrai si (x, y) hors [0,12)
inline bool isOutOfBoard2D(int x, int y) {
    return x < 0 || x >= 12 || y < 0 || y >= 12;
}

// Vrai si la case n'existe PAS (coin vide ou centre vide)
inline bool isSquareInvalid2D(int x, int y) {
    if (x < 4 && y > 7)  return true;  // coin haut droit vide
    if (x > 7 && y < 4)  return true;  // coin bas gauche vide
    if (x >= 4 && x < 8 && y >= 4 && y < 8) return true; // centre vide
    return false;
}

// Mur invisible du côté mat5 (bloque certaines traversées)
inline bool isWallMat5(int xStart, int yStart, int xMove, int yMove) {
    return (yStart == 7 && (yStart + yMove == 8)) &&
           ((xMove ==  0 && yMove == 1) ||
            (xMove == -1 && yMove == 1) ||
            (xMove ==  1 && yMove == 1));
}

// Mur invisible du côté mat4
inline bool isWallMat4(int xStart, int yStart, int xMove, int yMove) {
    return (xStart == 7 && (xStart + xMove == 8)) &&
           ((xMove == 1 && yMove == -1) ||
            (xMove == 1 && yMove ==  0) ||
            (xMove == 1 && yMove ==  1));
}

// Ajuste la destination aux passages de sous-matrice (style projetexp)
inline void adjustCoordinates2D(int xStart, int yStart,
                                 int& xDestination, int& yDestination,
                                 int& xMove, int& yMove) {
    int matrixStart       = determineSubMatrix2D(xStart, yStart);
    int matrixDestination = determineSubMatrix2D(xDestination, yDestination);

    // Traversée par le centre (4..7, 4..7) — zone interdite, on saute par-dessus
    if (xDestination > 3 && xDestination < 8 && yDestination > 3 && yDestination < 8) {
        if (xMove == -1 && yMove == 0) { xDestination -= 4; }
        else if (xMove == 0 && yMove == 1) { yDestination += 4; }
        else if (xMove == 1 && yMove == 0) { xDestination += 4; }
        else if (xMove == 0 && yMove == -1) { yDestination -= 4; }
        else if (xMove == -1 && yMove == -1) {
            if      (matrixStart == 4) yDestination -= 4;
            else if (matrixStart == 5) xDestination -= 4;
        }
        else if (xMove == -1 && yMove == 1) {
            if      (matrixStart == 3) yDestination += 4;
            else if (matrixStart == 5) xDestination -= 4;
        }
        else if (xMove == 1 && yMove == 1) {
            if      (matrixStart == 2) xDestination += 4;
            else if (matrixStart == 3) yDestination += 4;
        }
        else if (xMove == 1 && yMove == -1) {
            if      (matrixStart == 2) xDestination += 4;
            else if (matrixStart == 4) yDestination -= 4;
        }
    }
    // Inversions spécifiques entre matrices 5 et 6
    else if ((matrixStart == 6 && matrixDestination == 5) ||
             (matrixStart == 5 && yDestination == 3)) {
        yDestination = 11 - yDestination;
        if ((xMove == 0 || xMove == 1 || xMove == -1) && yMove == -1) yMove = 1;
    }
    // Inversions entre matrices 4 et 6
    else if ((matrixStart == 6 && matrixDestination == 4) ||
             (matrixStart == 4 && xDestination == 3)) {
        xDestination = 11 - xDestination;
        if (xMove == -1 && (yMove == 0 || yMove == -1 || yMove == 1)) xMove = 1;
    }
}
