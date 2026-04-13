#pragma once

#include "commandecoup.h"
#include "../model/jeu.h"
#include "../model/piece/piece.h"
#include "../model/base/position.h"

class CommandeDeplacement : public CommandeCoup {
protected:
    Jeu* jeu;
    Piece* piece;
    Position depart;
    Position arrivee;
    bool executionReussie;

public:
    CommandeDeplacement(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    void executer() override;
    void annuler() override;

    bool getExecutionReussie() const;
};