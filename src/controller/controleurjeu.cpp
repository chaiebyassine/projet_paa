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

    // Si aucune pièce n'est sélectionnée → on sélectionne
    if (!pieceSelectionnee) {
        selectionnerPiece(position);
    } 
    // Sinon → on essaie de déplacer la pièce sélectionnée
    else {
        demanderDeplacement(positionSelectionnee, position);
        pieceSelectionnee = false; // reset sélection
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

// Gère la création et l'exécution d'une commande
void ControleurJeu::demanderDeplacement(const Position& depart, const Position& arrivee) {

    if (jeu == nullptr) return;

    Case* caseDepart = jeu->getPlateau().obtenirCase(depart);
    Case* caseArrivee = jeu->getPlateau().obtenirCase(arrivee);

    // Vérifie que la case de départ contient une pièce
    if (caseDepart == nullptr || !caseDepart->estOccupee()) {
        return;
    }

    Piece* piece = caseDepart->getPiece();

    CommandeCoup* commande = nullptr;

    // Si la case d'arrivée est occupée → capture
    if (caseArrivee != nullptr && caseArrivee->estOccupee()) {
        commande = new CommandeCapture(jeu, piece, depart, arrivee);
    } 
    // Sinon → déplacement simple
    else {
        commande = new CommandeDeplacement(jeu, piece, depart, arrivee);
    }

    // Exécution de la commande
    commande->executer();

    // Ajout dans l'historique (important pour undo / IA)
    jeu->ajouterCommandeHistorique(commande);
}