#include "commandecapture.h"
#include "../model/plateau/case.h"

// Constructeur : initialise via la classe parente et met la pièce capturée à nullptr
CommandeCapture::CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee)
    : CommandeDeplacement(jeu, piece, depart, arrivee), pieceCapturee(nullptr) {
}

// Exécute la capture : sauvegarde la pièce présente sur la case d'arrivée, puis déplace
void CommandeCapture::executer() {
    if (jeu == nullptr) {
        return;
    }

    // Récupère la case d'arrivée pour sauvegarder la pièce qui va être capturée
    Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);
    if (caseArrivee != nullptr && caseArrivee->estOccupee()) {
        pieceCapturee = caseArrivee->getPiece();
    }

    // Effectue le déplacement (qui inclut la capture dans Jeu::deplacerPiece)
    executionReussie = jeu->deplacerPiece(depart, arrivee);
}

// Annule la capture : remet la pièce déplacée à sa position et replace la pièce capturée
void CommandeCapture::annuler() {
    if (jeu == nullptr || !executionReussie) {
        return;
    }

    // Remet la pièce déplacée à sa position de départ
    jeu->getPlateau().deplacerPiece(arrivee, depart);

    // Replace la pièce capturée sur la case d'arrivée originale
    if (pieceCapturee != nullptr) {
        Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);
        if (caseArrivee != nullptr) {
            caseArrivee->placerPiece(pieceCapturee);
        }
    }

    executionReussie = false;
}