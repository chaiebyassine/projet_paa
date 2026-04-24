#pragma once
#include <vector>
#include <map>
#include "../base/Position.h"
#include "Case.h"

class Piece;

// Plateau hexagonal Yalta 3 joueurs — 106 cases en coordonnées cubiques.
class Plateau {
private:
    std::vector<Case>          cases;
    std::map<Position, size_t> caseIndex;  // accès O(log n)

    void initialiserCases();
    void initialiserVoisins();

public:
    Plateau();

    bool caseExiste(const Position& pos) const;
    bool estCaseValide(const Position& pos) const;

    Case*       getCase(const Position& pos);
    const Case* getCase(const Position& pos) const;
    Case*       obtenirCase(const Position& pos);
    const Case* obtenirCase(const Position& pos) const;

    void placerPiece(const Position& pos, Piece* p);
    void retirerPiece(const Position& pos);
    void deplacerPiece(const Position& from, const Position& to);

    std::vector<Position> getVoisins(const Position& pos) const;
    std::vector<Position> getToutesLesPositions() const;
};
