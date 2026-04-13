#pragma once

#include <vector>
#include "plateau/Plateau.h"
#include "etatpartie.h"
#include "joueur/Joueur.h"
#include "base/position.h"
#include <vector>

class CommandeCoup;  // Déclaration anticipée de la classe CommandeCoup

// Classe principale du jeu d'échecs à 3 joueurs
// Gère le plateau, les joueurs, les tours de jeu, et la logique de la partie
class Jeu {
private:
    Plateau plateau;                          // Le plateau de jeu
    std::vector<Joueur*> joueurs;             // Liste des joueurs (max 3)
    int indexJoueurCourant;                   // Index du joueur dont c'est le tour
    EtatPartie etatPartie;                    // État actuel de la partie
    std::vector<CommandeCoup*> historiqueCoups; // Historique des coups joués (pour annuler)

public:
    // Constructeur : crée un jeu avec un plateau de taille lignes x colonnes
    Jeu(int lignes, int colonnes);

    // Initialise les pièces sur le plateau pour les 3 joueurs
    void initialiserPieces();

    // Ajoute un joueur au jeu (maximum 3 joueurs)
    void ajouterJoueur(Joueur* joueur);

    // Ajoute une commande à l'historique des coups
    void ajouterCommandeHistorique(CommandeCoup* commande);

    // Retourne l'historique des coups joués
    const std::vector<CommandeCoup*>& getHistoriqueCoups() const;

    // Retourne une référence modifiable vers le plateau
    Plateau& getPlateau();

    // Retourne une référence constante vers le plateau
    const Plateau& getPlateau() const;

    // Retourne le joueur dont c'est le tour
    Joueur* getJoueurCourant() const;

    // Retourne l'état actuel de la partie
    EtatPartie getEtatPartie() const;

    // Vérifie si un joueur est en échec (son roi est menacé)
    bool estEnEchec(Joueur* joueur);

    // Passe au joueur suivant
    void changerJoueur();

    // Déplace une pièce de la position de départ vers la position d'arrivée
    bool deplacerPiece(const Position& depart, const Position& arrivee);

    // Vérifie si un joueur est en échec et mat (aucun coup ne peut le sauver)
    bool estMat(Joueur* joueur);

    // Démarre une nouvelle partie
    void demarrerPartie();
};