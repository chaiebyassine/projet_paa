#pragma once

// Coordonnées cubiques hexagonales : q + r + s = 0
struct Position {
    int q, r, s;

    Position(int q = 0, int r = 0, int s = 0) : q(q), r(r), s(s) {}

    bool operator==(const Position& o) const { return q==o.q && r==o.r && s==o.s; }
    bool operator!=(const Position& o) const { return !(*this==o); }
    bool operator<(const Position& o) const {
        if (q != o.q) return q < o.q;
        if (r != o.r) return r < o.r;
        return s < o.s;
    }

    // Valide si et seulement si q+r+s == 0
    bool estValide() const { return q + r + s == 0; }
};

// Position sentinelle (invalide : somme != 0)
inline Position INVALID_POS() { return Position(1, 0, 0); }
