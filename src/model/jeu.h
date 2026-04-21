#pragma once

#include <vector>
#include "plateau/Plateau.h"
#include "etatpartie.h"
#include "joueur/Joueur.h"
#include "base/position.h"

class CommandeCoup;

// Classe principale du jeu : gère le plateau, les 3 joueurs, les tours et l'historique des coups
class Jeu {
private:
    Plateau plateau;                            // Le plateau de jeu avec toutes ses cases
    std::vector<Joueur*> joueurs;               // Les 3 joueurs de la partie
    int indexJoueurCourant;                     // Index (0, 1 ou 2) du joueur dont c'est le tour
    EtatPartie etatPartie;                      // État actuel : en cours, échec, mat, etc.
    std::vector<CommandeCoup*> historiqueCoups; // Tous les coups joués (permet d'annuler)

public:
    // Crée un jeu avec un plateau de taille lignes x colonnes
    Jeu(int lignes, int colonnes);

    // Place les 16 pièces de chaque joueur sur le plateau dans leur zone de départ
    void initialiserPieces();

    // Ajoute un joueur au jeu (3 maximum)
    void ajouterJoueur(Joueur* joueur);

    // Sauvegarde un coup dans l'historique (utile pour annuler)
    void ajouterCommandeHistorique(CommandeCoup* commande);

    // Retourne tous les coups enregistrés
    const std::vector<CommandeCoup*>& getHistoriqueCoups() const;

    // Retourne le plateau (version modifiable)
    Plateau& getPlateau();

    // Retourne le plateau (version lecture seule)
    const Plateau& getPlateau() const;

    // Retourne le joueur dont c'est le tour
    Joueur* getJoueurCourant() const;

    // Retourne l'état de la partie
    EtatPartie getEtatPartie() const;

    // Vérifie si le roi d'un joueur est menacé
    bool estEnEchec(Joueur* joueur);

    // Passe la main au joueur suivant
    void changerJoueur();

    // Déplace une pièce en vérifiant toutes les règles du jeu
    // Retourne vrai si le déplacement est valide et a été effectué
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    // Vérifie si un joueur est mat (aucun coup possible pour sortir de l'échec)
    bool estMat(Joueur* joueur);

    // Remet la partie à zéro et place les pièces
    void demarrerPartie();
};