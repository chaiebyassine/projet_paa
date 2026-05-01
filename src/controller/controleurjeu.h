#pragma once

#include "../model/jeu.h"
#include "../model/base/position.h"
#include <utility>

class Roi;
class Tour;

// Contrôleur du jeu (patron MVC).
// Traduit les clics utilisateur en commandes (CommandeDeplacement, CommandeCapture,
// CommandeRoque, CommandePromotion) et les envoie au modèle.
class ControleurJeu {
private:
    Jeu*     jeu;
    Position positionSelectionnee;
    bool     pieceSelectionnee;

    // Tente le roque entre le roi et la tour sélectionnés
    bool tenterRoque(Roi* roi, Tour* tour);

    // Calcule les positions d'arrivée du roi et de la tour pour un roque
    static std::pair<Position, Position> calculerPositionsRoque(Roi* roi, Tour* tour);

public:
    ControleurJeu(Jeu* jeu);

    void gererClicCase(const Position& position);
    void selectionnerPiece(const Position& position);
    bool demanderDeplacement(const Position& depart, const Position& arrivee);

    bool     aPieceSelectionnee()    const { return pieceSelectionnee; }
    Position getPositionSelectionnee() const { return positionSelectionnee; }
};