#pragma once

class Position {
private:
    int ligne;
    int colonne;

public:
    Position(int l, int c);

    int getLigne() const;
    int getColonne() const;

    bool estValide() const;
};