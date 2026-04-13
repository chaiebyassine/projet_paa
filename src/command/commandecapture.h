#pragma once

#include "commandedeplacement.h"

class CommandeCapture : public CommandeDeplacement {
private:
    Piece* pieceCapturee;

public:
    CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    void executer() override;
    void annuler() override;
};