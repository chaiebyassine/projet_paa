/**
 * @file controleurjeu.cpp
 * @brief Implémentation de la classe ControleurJeu.
 */
#include "controleurjeu.h"
#include "../model/plateau/case.h"
#include "../model/piece/piece.h"
#include "../command/commandedeplacement.h"
#include "../command/commandecapture.h"

// Constructeur : initialise le contrôleur
ControleurJeu::ControleurJeu(Jeu* jeu)
    : jeu(jeu), positionSelectionnee(Position(-1, -1)), pieceSelectionnee(false) {
}

// Méthode principale appelée lors d'un clic utilisateur
void ControleurJeu::gererClicCase(const Position& position) {

    if (!pieceSelectionnee) {
        selectionnerPiece(position);
    } else {
        // Clic sur une autre pièce du joueur courant → changer la sélection
        Case* c = jeu->getPlateau().obtenirCase(position);
        if (c && c->estOccupee() && c->getPiece()->getJoueur() == jeu->getJoueurCourant()) {
            positionSelectionnee = position;
            return;
        }
        // Tenter le déplacement ; si échec on conserve la sélection
        bool ok = demanderDeplacement(positionSelectionnee, position);
        if (ok) pieceSelectionnee = false;
    }
}

// Permet de sélectionner une pièce
void ControleurJeu::selectionnerPiece(const Position& position) {

    if (jeu == nullptr) return;

    // Récupère la case cliquée
    Case* c = jeu->getPlateau().obtenirCase(position);

    // Vérifie que la case est valide et contient une pièce
    if (c == nullptr || !c->estOccupee()) {
        return;
    }

    Piece* piece = c->getPiece();

    // Vérifie que la pièce appartient au joueur courant
    if (piece->getJoueur() == jeu->getJoueurCourant()) {
        positionSelectionnee = position;
        pieceSelectionnee = true;
    }
}

// Gère la création et l'exécution d'une commande ; retourne true si le coup réussit
bool ControleurJeu::demanderDeplacement(const Position& depart, const Position& arrivee) {

    if (jeu == nullptr) return false;

    Case* caseDepart  = jeu->getPlateau().obtenirCase(depart);
    Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);

    if (caseDepart == nullptr || !caseDepart->estOccupee()) return false;

    Piece* piece = caseDepart->getPiece();

    CommandeCoup* commande = nullptr;
    if (caseArrivee != nullptr && caseArrivee->estOccupee())
        commande = new CommandeCapture(jeu, piece, depart, arrivee);
    else
        commande = new CommandeDeplacement(jeu, piece, depart, arrivee);

    commande->executer();

    bool ok = commande->getExecutionReussie();
    if (ok)
        jeu->ajouterCommandeHistorique(commande);
    else
        delete commande;

    return ok;
}