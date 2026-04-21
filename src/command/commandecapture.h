#pragma once

#include "commandedeplacement.h"

// Commande qui déplace une pièce ET capture la pièce adverse sur la case d'arrivée
// Hérite de CommandeDeplacement et gère en plus la pièce capturée
class CommandeCapture : public CommandeDeplacement {
private:
    Piece* pieceCapturee;  // La pièce adverse capturée (sauvegardée pour pouvoir annuler)

public:
    // Crée la commande de capture
    CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    // Déplace la pièce et supprime la pièce adverse
    void executer() override;

    // Remet les deux pièces à leur place initiale
    void annuler() override;
};