#include "Jeu.h"
#include "plateau/Case.h"
#include "piece/Piece.h"
#include "piece/Tour.h"
#include "piece/Roi.h"
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
        // Retirer la pièce capturée du joueur adverse
        Joueur* joueurAdverse = pieceCapturee->getJoueur();
        joueurAdverse->retirerPiece(pieceCapturee);
        caseArrivee->vider();
    }

    plateau.deplacerPiece(depart, arrivee);
    changerJoueur();

    return true;
}

void Jeu::demarrerPartie() {
    etatPartie = EtatPartie::EN_COURS;
    indexJoueurCourant = 0;
    initialiserPieces();
}
// Initialise les pièces pour les 3 joueurs 
void Jeu::initialiserPieces() {

    // Vérifie qu'il y a bien 3 joueurs
    if (joueurs.size() < 3) {
        return;
    }

    Joueur* j1 = joueurs[0];
    Joueur* j2 = joueurs[1];
    Joueur* j3 = joueurs[2];

    // Placement des pièces du joueur 1
    // (positions temporaires pour test)
    Tour* tour1 = new Tour(Position(0, 0), j1->getCouleur(), j1);
    Roi* roi1 = new Roi(Position(0, 1), j1->getCouleur(), j1);

    j1->ajouterPiece(tour1);
    j1->ajouterPiece(roi1);

    plateau.placerPiece(Position(0, 0), tour1);
    plateau.placerPiece(Position(0, 1), roi1);

    // Même logique pour les autres joueurs...
    Tour* tour2 = new Tour(Position(7, 0), j2->getCouleur(), j2);
    Roi* roi2 = new Roi(Position(7, 1), j2->getCouleur(), j2);

    j2->ajouterPiece(tour2);
    j2->ajouterPiece(roi2);

    plateau.placerPiece(Position(7, 0), tour2);
    plateau.placerPiece(Position(7, 1), roi2);

    Tour* tour3 = new Tour(Position(0, 7), j3->getCouleur(), j3);
    Roi* roi3 = new Roi(Position(0, 6), j3->getCouleur(), j3);

    j3->ajouterPiece(tour3);
    j3->ajouterPiece(roi3);

    plateau.placerPiece(Position(0, 7), tour3);
    plateau.placerPiece(Position(0, 6), roi3);
    
}
Piece* trouverRoi(Joueur* joueur) {
    for (Piece* p : joueur->getPieces()) {
        // on suppose que le roi est identifié par son type
        if (dynamic_cast<Roi*>(p) != nullptr) {
            return p;
        }
    }
    return nullptr;
}
bool Jeu::estEnEchec(Joueur* joueur) {

    Piece* roi = trouverRoi(joueur);

    if (roi == nullptr) {
        return false;
    }

    Position positionRoi = roi->getPosition();

    // parcourir tous les autres joueurs
    for (Joueur* j : joueurs) {

        if (j == joueur) continue;

        // parcourir leurs pièces
        for (Piece* p : j->getPieces()) {

            std::vector<Position> mouvements = p->mouvementsPossibles(plateau);

            for (const Position& pos : mouvements) {
                if (pos.getLigne() == positionRoi.getLigne() &&
                    pos.getColonne() == positionRoi.getColonne()) {
                    return true;
                }
            }
        }
    }

    return false;
}