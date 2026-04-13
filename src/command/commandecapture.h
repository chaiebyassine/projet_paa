#pragma once

#include "commandedeplacement.h"

// Commande concrète pour un déplacement avec capture d'une pièce adverse
// Hérite de CommandeDeplacement et gère en plus la pièce capturée
class CommandeCapture : public CommandeDeplacement {
private:
    Piece* pieceCapturee;  // Pointeur vers la pièce capturée (pour pouvoir annuler)

public:
    // Constructeur : initialise la commande avec le jeu, la pièce, et les positions
    CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    // Exécute le déplacement avec capture de la pièce adverse
    void executer() override;

    // Annule la capture : remet la pièce déplacée et la pièce capturée à leur place
    void annuler() override;
};