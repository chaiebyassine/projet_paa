/**
 * @file joueur.cpp
 * @brief Implémentation de la classe abstraite Joueur.
 */
#include "Joueur.h"

// Constructeur : initialise le nom, la couleur, et le joueur n'est pas éliminé au départ
Joueur::Joueur(const std::string& nom, Couleur couleur)
    : nom(nom), couleur(couleur), estElimine(false) {
}

// Retourne le nom du joueur
std::string Joueur::getNom() const {
    return nom;
}

// Retourne la couleur du joueur
Couleur Joueur::getCouleur() const {
    return couleur;
}

// Retourne l'état d'élimination du joueur
bool Joueur::getEstElimine() const {
    return estElimine;
}

// Ajoute une pièce à la liste des pièces du joueur
void Joueur::ajouterPiece(Piece* p) {
    pieces.push_back(p);
}

// Retourne une copie de la liste des pièces du joueur
std::vector<Piece*> Joueur::getPieces() const {
    return pieces;
}

// Retire une pièce de la liste (utilisé quand une pièce est capturée)
void Joueur::retirerPiece(Piece* p) {
    // Parcourt la liste pour trouver la pièce et la supprimer
    for (auto it = pieces.begin(); it != pieces.end(); ++it) {
        if (*it == p) {
            pieces.erase(it);
            break;  // On sort dès qu'on l'a trouvée
        }
    }
}

void Joueur::setElimine(bool val) {
    estElimine = val;
}