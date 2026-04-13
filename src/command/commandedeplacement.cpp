#include "commandedeplacement.h"

CommandeDeplacement::CommandeDeplacement(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee)
    : jeu(jeu), piece(piece), depart(depart), arrivee(arrivee), executionReussie(false) {
}

void CommandeDeplacement::executer() {
    if (jeu != nullptr) {
        executionReussie = jeu->deplacerPiece(depart, arrivee);
    }
}

void CommandeDeplacement::annuler() {
    if (jeu != nullptr && executionReussie) {
        jeu->getPlateau().deplacerPiece(arrivee, depart);
        executionReussie = false;
    }
}

bool CommandeDeplacement::getExecutionReussie() const {
    return executionReussie;
}