#pragma once

#include "Position.h"
#include "Couleur.h"
#include "coord2d.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../piece/Piece.h"
#include <vector>

// Helpers de déplacement style projetexp (logique 2D sur grille 12×12).
// Tout est inline pour éviter de créer un .cpp additionnel.

namespace MoveHelper {

// Couleur de la pièce à (x, y), ou -1 si aucune
inline int sideAt(const Plateau& p, int x, int y) {
    Position pos = positionFrom2D(x, y);
    if (!pos.estValide()) return -1;
    const Case* c = p.obtenirCase(pos);
    if (!c || !c->estOccupee()) return -1;
    return static_cast<int>(c->getPiece()->getCouleur());
}

inline bool isEmpty(const Plateau& p, int x, int y) {
    Position pos = positionFrom2D(x, y);
    if (!pos.estValide()) return false;
    const Case* c = p.obtenirCase(pos);
    return c && !c->estOccupee();
}

inline bool isEnemy(const Plateau& p, int x, int y, Couleur myColor) {
    int s = sideAt(p, x, y);
    if (s < 0) return false;
    return s != static_cast<int>(myColor);
}

// Ajoute un mouvement si valide ; retourne true si on peut continuer (case libre)
inline bool tryAdd(const Plateau& p, int x, int y, Couleur myColor,
                   std::vector<Position>& out) {
    if (isOutOfBoard2D(x, y)) return false;
    if (isSquareInvalid2D(x, y)) return false;
    Position pos = positionFrom2D(x, y);
    if (!pos.estValide() || !p.caseExiste(pos)) return false;
    const Case* c = p.obtenirCase(pos);
    if (!c) return false;
    if (!c->estOccupee()) {
        out.push_back(pos);
        return true;
    }
    if (c->getPiece()->getCouleur() != myColor) out.push_back(pos);
    return false;
}

// Glissade récursive (Tour, Fou, Reine)
inline void recursiveMove(const Plateau& plateau,
                           int xStart, int yStart,
                           int xMove, int yMove,
                           Couleur myColor,
                           std::vector<Position>& moves)
{
    if (isWallMat5(xStart, yStart, xMove, yMove)) return;
    if (isWallMat4(xStart, yStart, xMove, yMove)) return;

    // Cas particuliers centraux (rosace Yalta)
    if (xStart == 3 && yStart == 3 && xMove == 1 && yMove == 1) {
        if (tryAdd(plateau, 4, 8, myColor, moves)) recursiveMove(plateau, 4, 8, xMove, yMove, myColor, moves);
        if (tryAdd(plateau, 8, 4, myColor, moves)) recursiveMove(plateau, 8, 4, xMove, yMove, myColor, moves);
        return;
    }
    if (xStart == 3 && yStart == 4 && xMove == 1 && yMove == -1) {
        if (tryAdd(plateau, 8, 8, myColor, moves)) recursiveMove(plateau, 8, 8, 1, 1, myColor, moves);
        if (tryAdd(plateau, 4, 3, myColor, moves)) recursiveMove(plateau, 4, 3, 1, -1, myColor, moves);
        return;
    }
    if (xStart == 4 && yStart == 3 && xMove == -1 && yMove == 1) {
        if (tryAdd(plateau, 8, 8, myColor, moves)) recursiveMove(plateau, 8, 8, 1, 1, myColor, moves);
        if (tryAdd(plateau, 3, 4, myColor, moves)) recursiveMove(plateau, 3, 4, -1, 1, myColor, moves);
        return;
    }
    if (xStart == 8 && yStart == 8 && xMove == -1 && yMove == -1) {
        if (tryAdd(plateau, 3, 4, myColor, moves)) recursiveMove(plateau, 3, 4, -1, 1, myColor, moves);
        if (tryAdd(plateau, 4, 3, myColor, moves)) recursiveMove(plateau, 4, 3, 1, -1, myColor, moves);
        return;
    }
    if (xStart == 8 && yStart == 4 && xMove == -1 && yMove == -1) {
        if (tryAdd(plateau, 4, 8, myColor, moves)) recursiveMove(plateau, 4, 8, 1, 1, myColor, moves);
        if (tryAdd(plateau, 3, 3, myColor, moves)) recursiveMove(plateau, 3, 3, -1, -1, myColor, moves);
        return;
    }
    if (xStart == 4 && yStart == 8 && xMove == -1 && yMove == -1) {
        if (tryAdd(plateau, 8, 4, myColor, moves)) recursiveMove(plateau, 8, 4, 1, 1, myColor, moves);
        if (tryAdd(plateau, 3, 3, myColor, moves)) recursiveMove(plateau, 3, 3, -1, -1, myColor, moves);
        return;
    }

    int xD = xStart + xMove;
    int yD = yStart + yMove;
    int xM = xMove, yM = yMove;
    adjustCoordinates2D(xStart, yStart, xD, yD, xM, yM);

    if (isOutOfBoard2D(xD, yD)) return;
    if (isSquareInvalid2D(xD, yD)) return;

    if (tryAdd(plateau, xD, yD, myColor, moves)) {
        recursiveMove(plateau, xD, yD, xM, yM, myColor, moves);
    }
}

// Déplacement d'une case (Roi) dans une direction donnée
inline void oneStep(const Plateau& plateau,
                     int xStart, int yStart,
                     int xMove, int yMove,
                     Couleur myColor,
                     std::vector<Position>& moves)
{
    if (isWallMat5(xStart, yStart, xMove, yMove)) return;
    if (isWallMat4(xStart, yStart, xMove, yMove)) return;

    // Cas particuliers centraux — doubles destinations
    if (xStart == 3 && yStart == 3 && xMove == 1 && yMove == 1) {
        tryAdd(plateau, 4, 8, myColor, moves);
        tryAdd(plateau, 8, 4, myColor, moves);
        return;
    }
    if (xStart == 3 && yStart == 4 && xMove == 1 && yMove == -1) {
        tryAdd(plateau, 8, 8, myColor, moves);
        tryAdd(plateau, 4, 3, myColor, moves);
        return;
    }
    if (xStart == 4 && yStart == 3 && xMove == -1 && yMove == 1) {
        tryAdd(plateau, 8, 8, myColor, moves);
        tryAdd(plateau, 3, 4, myColor, moves);
        return;
    }
    if (xStart == 8 && yStart == 8 && xMove == -1 && yMove == -1) {
        tryAdd(plateau, 3, 4, myColor, moves);
        tryAdd(plateau, 4, 3, myColor, moves);
        return;
    }
    if (xStart == 8 && yStart == 4 && xMove == -1 && yMove == -1) {
        tryAdd(plateau, 4, 8, myColor, moves);
        tryAdd(plateau, 3, 3, myColor, moves);
        return;
    }
    if (xStart == 4 && yStart == 8 && xMove == -1 && yMove == -1) {
        tryAdd(plateau, 8, 4, myColor, moves);
        tryAdd(plateau, 3, 3, myColor, moves);
        return;
    }

    int xD = xStart + xMove;
    int yD = yStart + yMove;
    int xM = xMove, yM = yMove;
    adjustCoordinates2D(xStart, yStart, xD, yD, xM, yM);

    if (isOutOfBoard2D(xD, yD)) return;
    if (isSquareInvalid2D(xD, yD)) return;

    tryAdd(plateau, xD, yD, myColor, moves);
}

// Helper interne : avance 1 case avec adjustCoordinates et contrôles de murs/bords.
// Retourne true et met à jour (x, y, mx, my) si valide ; false si bloqué.
inline bool advanceOne(int& x, int& y, int& mx, int& my) {
    if (isWallMat5(x, y, mx, my) || isWallMat4(x, y, mx, my)) return false;
    int xD = x + mx, yD = y + my;
    int mxA = mx, myA = my;
    adjustCoordinates2D(x, y, xD, yD, mxA, myA);
    if (isOutOfBoard2D(xD, yD) || isSquareInvalid2D(xD, yD)) return false;
    x = xD; y = yD; mx = mxA; my = myA;
    return true;
}

// Saut de cavalier : 2 cases dans (longDx, longDy) puis 1 case dans (shortDx, shortDy).
inline void knightHop(const Plateau& plateau,
                       int xStart, int yStart,
                       int longDx, int longDy,
                       int shortDx, int shortDy,
                       Couleur myColor,
                       std::vector<Position>& moves)
{
    int x = xStart, y = yStart;
    int mx = longDx, my = longDy;
    if (!advanceOne(x, y, mx, my)) return;
    if (!advanceOne(x, y, mx, my)) return;

    int mx2 = shortDx, my2 = shortDy;
    if (!advanceOne(x, y, mx2, my2)) return;

    Position dest = positionFrom2D(x, y);
    if (!dest.estValide() || !plateau.caseExiste(dest)) return;
    const Case* c = plateau.obtenirCase(dest);
    if (!c) return;
    if (!c->estOccupee() || c->getPiece()->getCouleur() != myColor) {
        moves.push_back(dest);
    }
}

// Direction d'avance du pion selon sa couleur et sa matrice courante (style projetexp).
// Retourne (xMove, 0) : le pion avance toujours le long de l'axe x.
inline std::pair<int,int> pawnDirection(int x, int y, Couleur c) {
    int mat  = determineSubMatrix2D(x, y);
    int side = (c == Couleur::BLANC) ? 1 : (c == Couleur::ROUGE) ? 2 : 3;
    bool plusOne =
        (side == 1) ||
        (side == 2 && (mat == 5 || mat == 6)) ||
        (side == 3 && (mat == 3 || mat == 4));
    return { plusOne ? 1 : -1, 0 };
}

// Déplacement d'un pion (avance + double initial + captures diagonales)
inline void pawnStep(const Plateau& plateau,
                      int xStart, int yStart,
                      Couleur myColor, bool hasMoved,
                      std::vector<Position>& moves)
{
    auto [xMove, yMove] = pawnDirection(xStart, yStart, myColor);
    if (xMove == 0 && yMove == 0) return;
    if (isWallMat4(xStart, yStart, xMove, yMove)) return;

    // Captures diagonales
    auto tryCapture = [&](int mx, int my) {
        if (isWallMat5(xStart, yStart, mx, my)) return;
        int xC = xStart + mx, yC = yStart + my;
        int mxA = mx, myA = my;
        adjustCoordinates2D(xStart, yStart, xC, yC, mxA, myA);
        if (isOutOfBoard2D(xC, yC) || isSquareInvalid2D(xC, yC)) return;
        Position pos = positionFrom2D(xC, yC);
        if (!pos.estValide() || !plateau.caseExiste(pos)) return;
        const Case* c = plateau.obtenirCase(pos);
        if (c && c->estOccupee() && c->getPiece()->getCouleur() != myColor)
            moves.push_back(pos);
    };

    // Les 2 diagonales : (xMove, ±1) ou (±1, yMove)
    if (xMove != 0) {
        tryCapture(xMove,  1);
        tryCapture(xMove, -1);
    } else {
        tryCapture( 1, yMove);
        tryCapture(-1, yMove);
    }

    // Avance 1 case
    int xD = xStart + xMove, yD = yStart + yMove;
    int mxA = xMove, myA = yMove;
    adjustCoordinates2D(xStart, yStart, xD, yD, mxA, myA);
    if (isOutOfBoard2D(xD, yD) || isSquareInvalid2D(xD, yD)) return;

    Position pos1 = positionFrom2D(xD, yD);
    if (!pos1.estValide() || !plateau.caseExiste(pos1)) return;
    const Case* c1 = plateau.obtenirCase(pos1);
    if (!c1 || c1->estOccupee()) return;
    moves.push_back(pos1);

    // Avance 2 cases si premier coup
    if (!hasMoved) {
        int xD2 = xD + mxA, yD2 = yD + myA;
        int mxA2 = mxA, myA2 = myA;
        adjustCoordinates2D(xD, yD, xD2, yD2, mxA2, myA2);
        if (isOutOfBoard2D(xD2, yD2) || isSquareInvalid2D(xD2, yD2)) return;
        Position pos2 = positionFrom2D(xD2, yD2);
        if (!pos2.estValide() || !plateau.caseExiste(pos2)) return;
        const Case* c2 = plateau.obtenirCase(pos2);
        if (c2 && !c2->estOccupee()) moves.push_back(pos2);
    }
}

} // namespace MoveHelper
