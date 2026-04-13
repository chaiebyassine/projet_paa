#include "commandecapture.h"
#include "../model/plateau/case.h"

CommandeCapture::CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee)
    : CommandeDeplacement(jeu, piece, depart, arrivee), pieceCapturee(nullptr) {
}

void CommandeCapture::executer() {
    if (jeu == nullptr) {
        return;
    }

    Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);
    if (caseArrivee != nullptr && caseArrivee->estOccupee()) {
        pieceCapturee = caseArrivee->getPiece();
    }

    executionReussie = jeu->deplacerPiece(depart, arrivee);
}

void CommandeCapture::annuler() {
    if (jeu == nullptr || !executionReussie) {
        return;
    }

    jeu->getPlateau().deplacerPiece(arrivee, depart);

    if (pieceCapturee != nullptr) {
        Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);
        if (caseArrivee != nullptr) {
            caseArrivee->placerPiece(pieceCapturee);
        }
    }

    executionReussie = false;
}