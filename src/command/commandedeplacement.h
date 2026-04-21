#pragma once

#include "commandecoup.h"
#include "../model/jeu.h"
#include "../model/piece/piece.h"
#include "../model/base/position.h"

// Commande qui déplace une pièce sans capture
// Sait s'annuler en remettant la pièce à sa position initiale
class CommandeDeplacement : public CommandeCoup {
protected:
    Jeu* jeu;               // Le jeu sur lequel on travaille
    Piece* piece;           // La pièce à déplacer
    Position depart;        // D'où part la pièce
    Position arrivee;       // Où elle va
    bool executionReussie;  // Vrai si le déplacement s'est bien passé

public:
    // Crée la commande avec tous les paramètres du déplacement
    CommandeDeplacement(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    // Lance le déplacement de la pièce
    void executer() override;

    // Remet la pièce à sa case de départ
    void annuler() override;

    // Retourne vrai si le déplacement a réussi
    bool getExecutionReussie() const;
};