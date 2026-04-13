#include "commandedeplacement.h"

// Constructeur : initialise tous les attributs, l'exécution n'a pas encore eu lieu
CommandeDeplacement::CommandeDeplacement(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee)
    : jeu(jeu), piece(piece), depart(depart), arrivee(arrivee), executionReussie(false) {
}

// Exécute le déplacement via le jeu, et stocke le résultat (réussi ou non)
void CommandeDeplacement::executer() {
    if (jeu != nullptr) {
        executionReussie = jeu->deplacerPiece(depart, arrivee);
    }
}

// Annule le déplacement en remettant la pièce à sa case de départ
void CommandeDeplacement::annuler() {
    if (jeu != nullptr && executionReussie) {
        // Déplace la pièce en sens inverse sur le plateau
        jeu->getPlateau().deplacerPiece(arrivee, depart);
        executionReussie = false;
    }
}

// Accesseur : retourne si le déplacement a été exécuté avec succès
bool CommandeDeplacement::getExecutionReussie() const {
    return executionReussie;
}