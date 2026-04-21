#pragma once

#include "../model/jeu.h"
#include "../model/base/position.h"

// Contrôleur du jeu (patron MVC)
// Reçoit les clics de l'utilisateur, crée les commandes et les envoie au modèle
class ControleurJeu {
private:
    Jeu* jeu;                        // Le modèle (contient toute la logique du jeu)
    Position positionSelectionnee;   // La case sur laquelle l'utilisateur a cliqué en premier
    bool pieceSelectionnee;          // Vrai si l'utilisateur a déjà choisi une pièce

public:
    // Crée le contrôleur avec une référence vers le jeu
    ControleurJeu(Jeu* jeu);

    // Appelé à chaque clic : 1er clic = sélection, 2ème clic = déplacement
    void gererClicCase(const Position& position);

    // Sélectionne la pièce à cette position si elle appartient au joueur courant
    void selectionnerPiece(const Position& position);

    // Crée et exécute la bonne commande (déplacement ou capture)
    void demanderDeplacement(const Position& depart, const Position& arrivee);

    // Indique si une pièce est actuellement sélectionnée (utile pour la vue)
    bool aPieceSelectionnee() const { return pieceSelectionnee; }

    // Retourne la position de la pièce sélectionnée (valide seulement si aPieceSelectionnee())
    Position getPositionSelectionnee() const { return positionSelectionnee; }
};