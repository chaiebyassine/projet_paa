#pragma once

#include "commandecoup.h"
#include "../model/jeu.h"
#include "../model/piece/piece.h"
#include "../model/base/position.h"

// Commande concrète pour un déplacement simple de pièce (sans capture)
// Hérite de CommandeCoup et implémente executer/annuler
class CommandeDeplacement : public CommandeCoup {
protected:
    Jeu* jeu;               // Référence vers le jeu pour effectuer le déplacement
    Piece* piece;           // La pièce à déplacer
    Position depart;        // Position de départ du mouvement
    Position arrivee;       // Position d'arrivée du mouvement
    bool executionReussie;  // Indique si le déplacement a réussi (utile pour annuler)

public:
    // Constructeur : initialise la commande avec le jeu, la pièce et les positions
    CommandeDeplacement(Jeu* jeu, Piece* piece, const Position& depart, const Position& arrivee);

    // Exécute le déplacement de la pièce sur le plateau
    void executer() override;

    // Annule le déplacement (remet la pièce à sa position initiale)
    void annuler() override;

    // Retourne vrai si l'exécution du déplacement a réussi
    bool getExecutionReussie() const;
};