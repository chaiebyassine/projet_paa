#pragma once

#include "../model/jeu.h"
#include "../model/base/position.h"

// Contrôleur du jeu (patron MVC)
class ControleurJeu {
private:
    Jeu* jeu;
    Position positionSelectionnee;
    bool pieceSelectionnee;

public:
    ControleurJeu(Jeu* jeu);

    void gererClicCase(const Position& position);
    void selectionnerPiece(const Position& position);

    // *** CORRECTION BUG #4 : retourne bool (succès ou échec) ***
    bool demanderDeplacement(const Position& depart, const Position& arrivee);

    bool aPieceSelectionnee() const { return pieceSelectionnee; }
    Position getPositionSelectionnee() const { return positionSelectionnee; }
};