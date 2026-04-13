#pragma once

#include <string>
#include <vector>
#include "../base/Couleur.h"

class Piece;  // Déclaration anticipée pour éviter l'inclusion circulaire

// Classe abstraite représentant un joueur du jeu d'échecs
// Sert de base pour JoueurHumain et JoueurIA
class Joueur {
protected:
    std::string nom;              // Nom du joueur
    Couleur couleur;              // Couleur attribuée au joueur (BLANC, NOIR ou ROUGE)
    bool estElimine;              // Indique si le joueur est éliminé de la partie
    std::vector<Piece*> pieces;   // Liste des pièces que le joueur possède encore

public:
    // Constructeur : crée un joueur avec un nom et une couleur
    Joueur(const std::string& nom, Couleur couleur);

    // Destructeur virtuel pour permettre la destruction correcte des sous-classes
    virtual ~Joueur() = default;

    // Retourne le nom du joueur
    std::string getNom() const;

    // Retourne la couleur du joueur
    Couleur getCouleur() const;

    // Retourne vrai si le joueur est éliminé
    bool getEstElimine() const;

    // Retire une pièce de la liste du joueur (après capture)
    void retirerPiece(Piece* p);

    // Ajoute une pièce à la liste du joueur
    void ajouterPiece(Piece* p);

    // Retourne la liste des pièces du joueur
    std::vector<Piece*> getPieces() const;

    // Méthode abstraite : chaque type de joueur implémente sa propre logique de jeu
    virtual void jouer() = 0;
};