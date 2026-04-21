#pragma once

#include <string>
#include <vector>
#include "../base/Couleur.h"

class Piece;

// Classe mère pour tous les joueurs (humain ou IA)
// Chaque joueur a un nom, une couleur, une liste de pièces et peut être éliminé
class Joueur {
protected:
    std::string nom;              // Nom du joueur
    Couleur couleur;              // Sa couleur (BLANC, NOIR ou ROUGE)
    bool estElimine;              // Vrai si ce joueur a été éliminé (mat)
    std::vector<Piece*> pieces;   // Liste de ses pièces encore en jeu

public:
    // Crée un joueur avec un nom et une couleur
    Joueur(const std::string& nom, Couleur couleur);

    virtual ~Joueur() = default;

    // Retourne le nom du joueur
    std::string getNom() const;

    // Retourne la couleur du joueur
    Couleur getCouleur() const;

    // Retourne vrai si le joueur est éliminé
    bool getEstElimine() const;

    // Enlève une pièce de la liste (quand elle est capturée)
    void retirerPiece(Piece* p);

    // Ajoute une pièce à la liste du joueur
    void ajouterPiece(Piece* p);

    // Retourne toutes les pièces du joueur encore en jeu
    std::vector<Piece*> getPieces() const;

    // Le joueur joue son tour (implémenté différemment selon humain ou IA)
    virtual void jouer() = 0;
};