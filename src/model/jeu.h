#pragma once

#include <vector>
#include "plateau/Plateau.h"
#include "etatpartie.h"
#include "joueur/Joueur.h"
#include "base/position.h"
#include <vector>
class CommandeCoup;
class Jeu {
private:
    Plateau plateau;
    std::vector<Joueur*> joueurs;
    int indexJoueurCourant;
    EtatPartie etatPartie;
std::vector<CommandeCoup*> historiqueCoups;
public:
    Jeu(int lignes, int colonnes);
    void initialiserPieces();

    void ajouterJoueur(Joueur* joueur);
void ajouterCommandeHistorique(CommandeCoup* commande);
const std::vector<CommandeCoup*>& getHistoriqueCoups() const;
    Plateau& getPlateau();
    const Plateau& getPlateau() const;

    Joueur* getJoueurCourant() const;
    EtatPartie getEtatPartie() const;
    bool estEnEchec(Joueur* joueur);
    void changerJoueur();
    bool deplacerPiece(const Position& depart, const Position& arrivee);
    bool estMat(Joueur* joueur);
    void demarrerPartie();
};