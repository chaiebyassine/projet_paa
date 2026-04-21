/**
 * @file commandecapture.cpp
 * @brief Implémentation de la classe CommandeCapture.
 */
#include "commandecapture.h"
#include "../model/plateau/case.h"

// Constructeur : initialise une commande de capture
// Hérite de CommandeDeplacement et initialise la pièce capturée à nullptr
CommandeCapture::CommandeCapture(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee)
    : CommandeDeplacement(jeu, piece, depart, arrivee), pieceCapturee(nullptr) {
}

// Exécution de la commande de capture
void CommandeCapture::executer() {

    // Vérifie que le jeu existe
    if (jeu == nullptr) {
        return;
    }

    // Récupère la case d'arrivée
    Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);

    // Si une pièce est présente sur la case, on la sauvegarde (pour pouvoir annuler)
    if (caseArrivee != nullptr && caseArrivee->estOccupee()) {
        pieceCapturee = caseArrivee->getPiece();
    }

    // Appelle la logique du jeu pour effectuer le déplacement (et la capture)
    executionReussie = jeu->deplacerPiece(depart, arrivee);
}

// Annulation de la capture
void CommandeCapture::annuler() {

    // Vérifie que la commande a bien été exécutée
    if (jeu == nullptr || !executionReussie) {
        return;
    }

    // Remet la pièce déplacée à sa position initiale
    jeu->getPlateau().deplacerPiece(arrivee, depart);

    // Si une pièce avait été capturée
    if (pieceCapturee != nullptr) {

        // Récupère la case d'arrivée
        Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);

        if (caseArrivee != nullptr) {

            // Remet la pièce capturée sur le plateau
            caseArrivee->placerPiece(pieceCapturee);

            // Important : remettre la pièce dans la liste du joueur adverse
            pieceCapturee->getJoueur()->ajouterPiece(pieceCapturee);
        }
    }

    // Marque la commande comme non exécutée
    executionReussie = false;
}