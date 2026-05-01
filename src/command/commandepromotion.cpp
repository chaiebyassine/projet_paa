/**
 * @file commandepromotion.cpp
 * @brief Implémentation de la promotion d'un Pion.
 */
#include "commandepromotion.h"
#include "../model/piece/Pion.h"
#include "../model/piece/Reine.h"
#include "../model/piece/Tour.h"
#include "../model/piece/Fou.h"
#include "../model/piece/Cavalier.h"
#include "../model/plateau/Plateau.h"
#include "../model/plateau/Case.h"
#include "../model/joueur/Joueur.h"

CommandePromotion::CommandePromotion(Jeu* jeu, Pion* pion, TypePromotion type)
    : jeu(jeu), pion(pion), typePromotion(type),
      piecePromue(nullptr), executionReussie(false) {
}

Piece* CommandePromotion::creerPiecePromue() const {
    Position pos = pion->getPosition();
    Couleur  col = pion->getCouleur();
    Joueur*  j   = pion->getJoueur();
    switch (typePromotion) {
        case TypePromotion::TOUR:     return new Tour    (pos, col, j);
        case TypePromotion::FOU:      return new Fou     (pos, col, j);
        case TypePromotion::CAVALIER: return new Cavalier(pos, col, j);
        default:                      return new Reine   (pos, col, j);
    }
}

void CommandePromotion::executer() {
    if (!jeu || !pion) return;
    if (!pion->peutEtrePromu()) return;

    Position pos = pion->getPosition();
    Joueur*  j   = pion->getJoueur();
    Plateau& pl  = jeu->getPlateau();

    // Retirer le pion du plateau et de la liste du joueur
    pl.retirerPiece(pos);
    j->retirerPiece(pion);

    // Créer et placer la pièce promue
    piecePromue = creerPiecePromue();
    pl.placerPiece(pos, piecePromue);
    j->ajouterPiece(piecePromue);

    executionReussie = true;
}

void CommandePromotion::annuler() {
    if (!executionReussie || !piecePromue) return;

    Position pos = pion->getPosition();
    Joueur*  j   = pion->getJoueur();
    Plateau& pl  = jeu->getPlateau();

    // Retirer la pièce promue du plateau et de la liste du joueur
    pl.retirerPiece(pos);
    j->retirerPiece(piecePromue);

    // Remettre le pion
    pl.placerPiece(pos, pion);
    j->ajouterPiece(pion);

    delete piecePromue;
    piecePromue = nullptr;
    executionReussie = false;
}

bool CommandePromotion::getExecutionReussie() const {
    return executionReussie;
}
