#include "Jeu.h"
#include "plateau/Case.h"
#include "piece/Piece.h"
#include "piece/Tour.h"
#include "piece/Roi.h"
#include "piece/Pion.h"
#include "piece/Cavalier.h"
#include "piece/Fou.h"
#include "piece/Reine.h"
#include "joueur/Joueur.h"

Jeu::Jeu() : plateau(), indexJoueurCourant(0), etatPartie(EtatPartie::EN_COURS) {}

void Jeu::ajouterJoueur(Joueur* joueur) {
    if (joueurs.size() < 3) joueurs.push_back(joueur);
}

Plateau& Jeu::getPlateau()             { return plateau; }
const Plateau& Jeu::getPlateau() const { return plateau; }

Joueur* Jeu::getJoueurCourant() const {
    if (joueurs.empty()) return nullptr;
    return joueurs[indexJoueurCourant];
}

EtatPartie Jeu::getEtatPartie() const { return etatPartie; }

void Jeu::changerJoueur() {
    if (!joueurs.empty())
        indexJoueurCourant = (indexJoueurCourant + 1) % joueurs.size();
}

bool Jeu::deplacerPiece(const Position& depart, const Position& arrivee) {
    Case* caseDepart  = plateau.obtenirCase(depart);
    Case* caseArrivee = plateau.obtenirCase(arrivee);
    if (!caseDepart || !caseArrivee) return false;
    if (!caseDepart->estOccupee()) return false;

    Piece* piece = caseDepart->getPiece();
    if (piece->getJoueur() != getJoueurCourant()) return false;

    auto mouvements = piece->mouvementsPossibles(plateau);
    bool ok = false;
    for (const auto& p : mouvements) if (p == arrivee) { ok = true; break; }
    if (!ok) return false;

    if (caseArrivee->estOccupee()) {
        Piece* cap = caseArrivee->getPiece();
        if (cap->getCouleur() == piece->getCouleur()) return false;
        cap->getJoueur()->retirerPiece(cap);
        caseArrivee->vider();
    }

    plateau.deplacerPiece(depart, arrivee);
    piece->setADejaBouge(true);
    changerJoueur();
    return true;
}

void Jeu::demarrerPartie() {
    etatPartie = EtatPartie::EN_COURS;
    indexJoueurCourant = 0;
    initialiserPieces();
}

// Place les pièces selon les positions exactes du jeu Yalta 3 joueurs.
// Chaque joueur a 8 pièces (arrière-ban) + 8 pions.
//
// BLANC (joueurs[0]) — rangée arrière s=4, pions s=3
// ROUGE (joueurs[1]) — rangée arrière q=4, pions q=3
// NOIR  (joueurs[2]) — rangée arrière r=4, pions r=3
void Jeu::initialiserPieces() {
    if (joueurs.size() < 3) return;

    const Couleur couleurs[3] = { Couleur::BLANC, Couleur::ROUGE, Couleur::NOIR };

    // Rangée arrière de chaque joueur : Tour-Cav-Fou-Dame-Roi-Fou-Cav-Tour
    // Les 8 positions sont listées dans l'ordre pour chaque joueur.
    const Position arriere[3][8] = {
        // BLANC (s=4)
        { {-4, 0,4}, {-3,-1,4}, {-2,-2,4}, {-1,-3,4},
          { 0,-4,4}, { 1,-5,4}, { 2,-6,4}, { 3,-7,4} },
        // ROUGE (q=4)
        { {4,-4,0}, {4,-3,-1}, {4,-2,-2}, {4,-1,-3},
          {4, 0,-4}, {4, 1,-5}, {4, 2,-6}, {4, 3,-7} },
        // NOIR (r=4)
        { { 0,4,-4}, {-1,4,-3}, {-2,4,-2}, {-3,4,-1},
          {-4,4, 0}, {-5,4, 1}, {-6,4, 2}, {-7,4, 3} }
    };

    const Position pions[3][8] = {
        // BLANC (s=3)
        { {-3, 0,3}, {-2,-1,3}, {-1,-2,3}, { 0,-3,3},
          { 1,-4,3}, { 2,-5,3}, { 3,-6,3}, { 4,-7,3} },
        // ROUGE (q=3)
        { {3,-3,0}, {3,-2,-1}, {3,-1,-2}, {3, 0,-3},
          {3, 1,-4}, {3, 2,-5}, {3, 3,-6}, {3, 4,-7} },
        // NOIR (r=3)
        { { 0,3,-3}, {-1,3,-2}, {-2,3,-1}, {-3,3, 0},
          {-4,3, 1}, {-5,3, 2}, {-6,3, 3}, {-7,3, 4} }
    };

    auto placer = [&](Joueur* j, Piece* p) {
        j->ajouterPiece(p);
        plateau.placerPiece(p->getPosition(), p);
    };

    for (int k = 0; k < 3; k++) {
        Joueur* j  = joueurs[k];
        Couleur col = couleurs[k];

        // Rangée arrière : Tour Cav Fou Dame Roi Fou Cav Tour
        placer(j, new Tour    (arriere[k][0], col, j));
        placer(j, new Cavalier(arriere[k][1], col, j));
        placer(j, new Fou     (arriere[k][2], col, j));
        placer(j, new Reine   (arriere[k][3], col, j));
        placer(j, new Roi     (arriere[k][4], col, j));
        placer(j, new Fou     (arriere[k][5], col, j));
        placer(j, new Cavalier(arriere[k][6], col, j));
        placer(j, new Tour    (arriere[k][7], col, j));

        // Pions
        for (int i = 0; i < 8; i++)
            placer(j, new Pion(pions[k][i], col, j));
    }
}

static Piece* trouverRoi(Joueur* joueur) {
    for (Piece* p : joueur->getPieces())
        if (dynamic_cast<Roi*>(p)) return p;
    return nullptr;
}

bool Jeu::estEnEchec(Joueur* joueur) {
    Piece* roi = trouverRoi(joueur);
    if (!roi) return false;
    Position posRoi = roi->getPosition();
    for (Joueur* j : joueurs) {
        if (j == joueur) continue;
        for (Piece* p : j->getPieces()) {
            for (const Position& pos : p->mouvementsPossibles(plateau))
                if (pos == posRoi) return true;
        }
    }
    return false;
}

bool Jeu::estMat(Joueur* joueur) {
    if (!estEnEchec(joueur)) return false;
    for (Piece* piece : joueur->getPieces()) {
        for (const Position& arrivee : piece->mouvementsPossibles(plateau)) {
            Position depart = piece->getPosition();
            Case* cd = plateau.obtenirCase(depart);
            Case* ca = plateau.obtenirCase(arrivee);
            if (!cd || !ca) continue;

            Piece* cap = ca->getPiece();
            bool occupied = ca->estOccupee();

            cd->vider();
            ca->placerPiece(piece);
            piece->setPosition(arrivee);

            bool encoreEnEchec = estEnEchec(joueur);

            ca->vider();
            cd->placerPiece(piece);
            piece->setPosition(depart);
            if (occupied) ca->placerPiece(cap);

            if (!encoreEnEchec) return false;
        }
    }
    return true;
}

void Jeu::ajouterCommandeHistorique(CommandeCoup* commande) {
    historiqueCoups.push_back(commande);
}
const std::vector<CommandeCoup*>& Jeu::getHistoriqueCoups() const {
    return historiqueCoups;
}
