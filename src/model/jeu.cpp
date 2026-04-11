#include "Jeu.h"
#include "plateau/Case.h"
#include "piece/Piece.h"

Jeu::Jeu(int lignes, int colonnes)
    : plateau(lignes, colonnes), indexJoueurCourant(0), etatPartie(EtatPartie::EN_COURS) {
}

void Jeu::ajouterJoueur(Joueur* joueur) {
    if (joueurs.size() < 3) {
        joueurs.push_back(joueur);
    }
}

Plateau& Jeu::getPlateau() {
    return plateau;
}

const Plateau& Jeu::getPlateau() const {
    return plateau;
}

Joueur* Jeu::getJoueurCourant() const {
    if (joueurs.empty()) {
        return nullptr;
    }
    return joueurs[indexJoueurCourant];
}

EtatPartie Jeu::getEtatPartie() const {
    return etatPartie;
}

void Jeu::changerJoueur() {
    if (joueurs.empty()) {
        return;
    }

    indexJoueurCourant = (indexJoueurCourant + 1) % joueurs.size();
}

bool Jeu::deplacerPiece(const Position& depart, const Position& arrivee) {
    Case* caseDepart = plateau.obtenirCase(depart);
    Case* caseArrivee = plateau.obtenirCase(arrivee);

    if (caseDepart == nullptr || caseArrivee == nullptr) {
        return false;
    }

    if (!caseDepart->estOccupee()) {
        return false;
    }

    Piece* piece = caseDepart->getPiece();

    if (piece->getJoueur() != getJoueurCourant()) {
        return false;
    }

    std::vector<Position> mouvements = piece->mouvementsPossibles(plateau);
    bool mouvementValide = false;

    for (const Position& p : mouvements) {
        if (p.getLigne() == arrivee.getLigne() && p.getColonne() == arrivee.getColonne()) {
            mouvementValide = true;
            break;
        }
    }

    if (!mouvementValide) {
        return false;
    }

    // Capture simple si une pièce adverse est sur la case d’arrivée
    if (caseArrivee->estOccupee()) {
        Piece* pieceCapturee = caseArrivee->getPiece();
        if (pieceCapturee->getCouleur() == piece->getCouleur()) {
            return false;
        }
    }

    plateau.deplacerPiece(depart, arrivee);
    changerJoueur();

    return true;
}

void Jeu::demarrerPartie() {
    etatPartie = EtatPartie::EN_COURS;
    indexJoueurCourant = 0;
}