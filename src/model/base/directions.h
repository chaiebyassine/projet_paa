#pragma once
#include "Position.h"
#include "Couleur.h"
#include <array>

// ============================================================
// Directions sur plateau hexagonal (coordonnées cubiques).
// Chaque direction est un vecteur (dq, dr, ds) avec dq+dr+ds=0.
//
// 6 directions orthogonales (Tour / Roi)
//   D1 (+1,-1, 0)  D2 (-1,+1, 0)
//   D3 (+1, 0,-1)  D4 (-1, 0,+1)   ← D3 = avance BLANC
//   D5 ( 0,+1,-1)  D6 ( 0,-1,+1)   ← D6 = avance NOIR / D2 = avance ROUGE
//
// 6 directions diagonales (Fou)
//   B1 (+2,-1,-1)  B2 (-2,+1,+1)
//   B3 (+1,-2,+1)  B4 (-1,+2,-1)
//   B5 (+1,+1,-2)  B6 (-1,-1,+2)
//
// 12 sauts de cavalier
// ============================================================

using Dir3 = std::array<int,3>;

static const Dir3 HEX_DIRS[6] = {
    { 1,-1, 0},  // D1
    {-1, 1, 0},  // D2  ROUGE advance
    { 1, 0,-1},  // D3  BLANC advance
    {-1, 0, 1},  // D4
    { 0, 1,-1},  // D5
    { 0,-1, 1}   // D6  NOIR advance
};

static const Dir3 DIAG_DIRS[6] = {
    { 2,-1,-1},  // B1
    {-2, 1, 1},  // B2
    { 1,-2, 1},  // B3
    {-1, 2,-1},  // B4
    { 1, 1,-2},  // B5
    {-1,-1, 2}   // B6
};

static const Dir3 KNIGHT_MOVES[12] = {
    { 3,-2,-1}, { 3,-1,-2},
    {-3, 2, 1}, {-3, 1, 2},
    { 2, 1,-3}, { 1, 2,-3},
    {-2,-1, 3}, {-1,-2, 3},
    { 2,-3, 1}, {-2, 3,-1},
    { 1,-3, 2}, {-1, 3,-2}
};

inline Position step(const Position& p, const Dir3& d) {
    return Position(p.q + d[0], p.r + d[1], p.s + d[2]);
}

// Directions d'avance et de capture du pion selon la couleur
struct PawnInfo {
    Dir3 avance;
    Dir3 cap1;
    Dir3 cap2;
};

inline PawnInfo pawnInfo(Couleur c) {
    if (c == Couleur::BLANC)
        return { { 1, 0,-1}, { 1,-1, 0}, { 0, 1,-1} };
    if (c == Couleur::ROUGE)
        return { {-1, 1, 0}, {-1, 0, 1}, { 0, 1,-1} };
    // NOIR
    return     { { 0,-1, 1}, { 1,-1, 0}, {-1, 0, 1} };
}
