#include "plateau.h"
#include "../piece/Piece.h"
Plateau::Plateau(int lignes, int colonnes)
    : nbLignes(lignes), nbColonnes(colonnes) {
    for (int i = 0; i < nbLignes; ++i) {
        for (int j = 0; j < nbColonnes; ++j) {
            cases.push_back(Case(Position(i, j)));
        }
    }
}

bool Plateau::estCaseValide(const Position& p) const {
    return p.getLigne() >= 0 && p.getLigne() < nbLignes &&
           p.getColonne() >= 0 && p.getColonne() < nbColonnes;
}

Case* Plateau::obtenirCase(const Position& p) {
    if (!estCaseValide(p)) {
        return nullptr;
    }

    int index = p.getLigne() * nbColonnes + p.getColonne();
    return &cases[index];
}

const Case* Plateau::obtenirCase(const Position& p) const {
    if (!estCaseValide(p)) {
        return nullptr;
    }

    int index = p.getLigne() * nbColonnes + p.getColonne();
    return &cases[index];
}

bool Plateau::placerPiece(const Position& p, Piece* piece) {
    Case* c = obtenirCase(p);
    if (c == nullptr) {
        return false;
    }

    c->placerPiece(piece);
    return true;
}

bool Plateau::deplacerPiece(const Position& depart, const Position& arrivee) {
    Case* caseDepart = obtenirCase(depart);
    Case* caseArrivee = obtenirCase(arrivee);

    if (caseDepart == nullptr || caseArrivee == nullptr) {
        return false;
    }

    if (!caseDepart->estOccupee()) {
        return false;
    }

    Piece* piece = caseDepart->getPiece();
piece->setPosition(arrivee);
piece->setADejaBouge(true);
caseArrivee->placerPiece(piece);
caseDepart->vider();

    return true;
}

int Plateau::getNbLignes() const {
    return nbLignes;
}

int Plateau::getNbColonnes() const {
    return nbColonnes;
}