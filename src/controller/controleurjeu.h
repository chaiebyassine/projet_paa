#pragma once

#include "../model/jeu.h"
#include "../model/base/position.h"

// Controleur du jeu (MVC)
// Il reçoit les actions utilisateur (clics) et déclenche les commandes
class ControleurJeu {
private:
    Jeu* jeu;                         // Référence vers le modèle (Jeu)
    Position positionSelectionnee;   // Position de la pièce sélectionnée
    bool pieceSelectionnee;          // Indique si une pièce est sélectionnée

public:
    // Constructeur
    ControleurJeu(Jeu* jeu);

    // Gère un clic sur une case
    void gererClicCase(const Position& position);

    // Sélectionne une pièce
    void selectionnerPiece(const Position& position);

    // Demande un déplacement (création de commande)
    void demanderDeplacement(const Position& depart, const Position& arrivee);
};