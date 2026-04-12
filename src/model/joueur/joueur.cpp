#include "Joueur.h"

Joueur::Joueur(const std::string& nom, Couleur couleur)
    : nom(nom), couleur(couleur), estElimine(false) {
}

std::string Joueur::getNom() const {
    return nom;
}

Couleur Joueur::getCouleur() const {
    return couleur;
}

bool Joueur::getEstElimine() const {
    return estElimine;
}

void Joueur::ajouterPiece(Piece* p) {
    pieces.push_back(p);
}

std::vector<Piece*> Joueur::getPieces() const {
    return pieces;
}
void Joueur::retirerPiece(Piece* p) {
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if (*it == p) {
            pieces.erase(it);
            break;
        }
    }
}