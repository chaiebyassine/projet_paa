#pragma once

#include <vector>
#include "plateau/Plateau.h"
#include "etatpartie.h"
#include "joueur/Joueur.h"
#include "base/position.h"

class Jeu {
private:
    Plateau plateau;
    std::vector<Joueur*> joueurs;
    int indexJoueurCourant;
    EtatPartie etatPartie;

public:
    Jeu(int lignes, int colonnes);

    void ajouterJoueur(Joueur* joueur);

    Plateau& getPlateau();
    const Plateau& getPlateau() const;

    Joueur* getJoueurCourant() const;
    EtatPartie getEtatPartie() const;

    void changerJoueur();
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    void demarrerPartie();
};