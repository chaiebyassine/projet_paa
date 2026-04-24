#include "Plateau.h"
#include "../piece/Piece.h"
#include "../base/directions.h"
#include <set>

Plateau::Plateau() {
    initialiserCases();
    initialiserVoisins();
}

// Enumère les 96 cases valides du plateau Yalta 3 joueurs (3 zones disjointes de 32).
// Zone BLANC : depth d∈[0,3] (s=4-d), col c∈[0,7] (r=-c), q=c+d-4   → 32 cases
// Zone ROUGE : depth d∈[0,3] (q=4-d), col c∈[0,7] (s=-c), r=c+d-4   → 32 cases
// Zone NOIR  : depth d∈[0,3] (r=4-d), col c∈[0,7] (q=-c), s=c+d-4   → 32 cases
void Plateau::initialiserCases() {
    cases.clear();
    caseIndex.clear();

    std::set<Position> valid;

    // BLANC
    for (int d = 0; d <= 3; d++)
        for (int c = 0; c <= 7; c++)
            valid.insert(Position(c + d - 4, -c, 4 - d));

    // ROUGE
    for (int d = 0; d <= 3; d++)
        for (int c = 0; c <= 7; c++)
            valid.insert(Position(4 - d, c + d - 4, -c));

    // NOIR
    for (int d = 0; d <= 3; d++)
        for (int c = 0; c <= 7; c++)
            valid.insert(Position(-c, 4 - d, c + d - 4));

    cases.reserve(valid.size());
    for (const auto& p : valid) {
        caseIndex[p] = cases.size();
        cases.emplace_back(p);
    }
}

void Plateau::initialiserVoisins() {
    for (auto& c : cases) {
        const Position& p = c.getPosition();
        for (const auto& d : HEX_DIRS) {
            Position n(p.q + d[0], p.r + d[1], p.s + d[2]);
            if (caseExiste(n))
                c.ajouterVoisin(n);
        }
    }
}

bool Plateau::caseExiste(const Position& pos) const {
    return caseIndex.count(pos) > 0;
}
bool Plateau::estCaseValide(const Position& pos) const { return caseExiste(pos); }

Case* Plateau::getCase(const Position& pos) {
    auto it = caseIndex.find(pos);
    return (it != caseIndex.end()) ? &cases[it->second] : nullptr;
}
const Case* Plateau::getCase(const Position& pos) const {
    auto it = caseIndex.find(pos);
    return (it != caseIndex.end()) ? &cases[it->second] : nullptr;
}
Case*       Plateau::obtenirCase(const Position& pos)       { return getCase(pos); }
const Case* Plateau::obtenirCase(const Position& pos) const { return getCase(pos); }

void Plateau::placerPiece(const Position& pos, Piece* p) {
    Case* c = getCase(pos);
    if (c) c->setPiece(p);
}
void Plateau::retirerPiece(const Position& pos) {
    Case* c = getCase(pos);
    if (c) c->vider();
}
void Plateau::deplacerPiece(const Position& from, const Position& to) {
    Case* src = getCase(from);
    Case* dst = getCase(to);
    if (!src || !dst) return;
    Piece* p = src->getPiece();
    src->vider();
    if (p) dst->placerPiece(p);
}

std::vector<Position> Plateau::getVoisins(const Position& pos) const {
    const Case* c = getCase(pos);
    return c ? c->getVoisins() : std::vector<Position>{};
}
std::vector<Position> Plateau::getToutesLesPositions() const {
    std::vector<Position> res;
    res.reserve(cases.size());
    for (const auto& c : cases) res.push_back(c.getPosition());
    return res;
}
