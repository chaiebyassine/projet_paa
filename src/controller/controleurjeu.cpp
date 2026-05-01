/**
 * @file controleurjeu.cpp
 * @brief Contrôleur MVC : traduit les clics en commandes (Deplacement, Capture, Roque).
 */
#include "controleurjeu.h"
#include "../model/plateau/case.h"
#include "../model/piece/piece.h"
#include "../model/piece/Roi.h"
#include "../model/piece/Tour.h"
#include "../command/commandedeplacement.h"
#include "../command/commandecapture.h"
#include "../command/commanderoque.h"
#include "../model/base/coord2d.h"

ControleurJeu::ControleurJeu(Jeu* jeu)
    : jeu(jeu), positionSelectionnee(Position(-1, -1)), pieceSelectionnee(false) {
}

// ── Gestion du clic ──────────────────────────────────────────────────────────

void ControleurJeu::gererClicCase(const Position& position) {
    if (!pieceSelectionnee) {
        selectionnerPiece(position);
        return;
    }

    Case* cCible = jeu->getPlateau().obtenirCase(position);
    Case* cSel   = jeu->getPlateau().obtenirCase(positionSelectionnee);
    Piece* sel   = (cSel && cSel->estOccupee()) ? cSel->getPiece() : nullptr;

    // Clic sur une pièce du joueur courant
    if (cCible && cCible->estOccupee() &&
        cCible->getPiece()->getJoueur() == jeu->getJoueurCourant()) {

        // Cas spécial : Roi sélectionné + clic sur sa propre Tour → tentative de roque
        Roi*  roi  = dynamic_cast<Roi*>(sel);
        Tour* tour = dynamic_cast<Tour*>(cCible->getPiece());
        if (roi && tour) {
            if (tenterRoque(roi, tour))
                pieceSelectionnee = false;
            return;
        }

        // Sinon : changer de sélection
        positionSelectionnee = position;
        return;
    }

    // Déplacement normal ou capture
    bool ok = demanderDeplacement(positionSelectionnee, position);
    if (ok) pieceSelectionnee = false;
}

void ControleurJeu::selectionnerPiece(const Position& position) {
    if (!jeu) return;
    Case* c = jeu->getPlateau().obtenirCase(position);
    if (!c || !c->estOccupee()) return;
    if (c->getPiece()->getJoueur() == jeu->getJoueurCourant()) {
        positionSelectionnee = position;
        pieceSelectionnee    = true;
    }
}

// ── Déplacement / Capture ────────────────────────────────────────────────────

bool ControleurJeu::demanderDeplacement(const Position& depart, const Position& arrivee) {
    if (!jeu) return false;

    Case* cDepart  = jeu->getPlateau().obtenirCase(depart);
    Case* cArrivee = jeu->getPlateau().obtenirCase(arrivee);
    if (!cDepart || !cDepart->estOccupee()) return false;

    Piece* piece = cDepart->getPiece();

    CommandeCoup* commande = nullptr;
    if (cArrivee && cArrivee->estOccupee())
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

// ── Roque ────────────────────────────────────────────────────────────────────

bool ControleurJeu::tenterRoque(Roi* roi, Tour* tour) {
    if (!CommandeRoque::peutRoquer(*jeu, roi, tour)) return false;

    auto [posRoiArr, posTourArr] = calculerPositionsRoque(roi, tour);
    if (!posRoiArr.estValide() || !posTourArr.estValide()) return false;

    CommandeRoque* cmd = new CommandeRoque(jeu, roi, tour, posRoiArr, posTourArr);
    cmd->executer();

    if (cmd->getExecutionReussie()) {
        jeu->ajouterCommandeHistorique(cmd);
        jeu->changerJoueur();
        return true;
    }
    delete cmd;
    return false;
}

std::pair<Position, Position>
ControleurJeu::calculerPositionsRoque(Roi* roi, Tour* tour) {
    auto [xR, yR] = positionTo2D(roi->getPosition());
    auto [xT, yT] = positionTo2D(tour->getPosition());

    Position posRoiArrivee  = INVALID_POS();
    Position posTourArrivee = INVALID_POS();

    if (xR == xT) {
        if (yT > yR) {
            posRoiArrivee  = positionFrom2D(xR, yR + 2);
            posTourArrivee = positionFrom2D(xR, yR + 1);
        } else {
            posRoiArrivee  = positionFrom2D(xR, yR - 2);
            posTourArrivee = positionFrom2D(xR, yR - 1);
        }
    } else if (yR == yT) {
        if (xT > xR) {
            posRoiArrivee  = positionFrom2D(xR + 2, yR);
            posTourArrivee = positionFrom2D(xR + 1, yR);
        } else {
            posRoiArrivee  = positionFrom2D(xR - 2, yR);
            posTourArrivee = positionFrom2D(xR - 1, yR);
        }
    }

    return { posRoiArrivee, posTourArrivee };
}
