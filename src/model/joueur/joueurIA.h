#pragma once

#include "Joueur.h"

// Représente un joueur contrôlé par l'intelligence artificielle
class JoueurIA : public Joueur {
private:
    int profondeurRecherche;  // Profondeur de l'algorithme de recherche (minimax, etc.)

public:
    // Constructeur : crée un joueur IA avec un nom, une couleur et une profondeur de recherche
    JoueurIA(const std::string& nom, Couleur couleur, int profondeur);

    // Implémentation de la méthode jouer pour un joueur IA
    void jouer() override;
};