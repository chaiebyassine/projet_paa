/**
 * @file main.cpp
 * @brief Point d'entrée de l'application — Jeu d'échecs Yalta à 3 joueurs.
 *
 * Crée un jeu, ajoute les joueurs, place une pièce de test et simule
 * deux clics via le contrôleur pour valider le déplacement et l'historique.
 */
#include <iostream>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/model/piece/tour.h"
#include "src/controller/controleurjeu.h"

int main() {
    Jeu jeu(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);
    JoueurHumain j3("Joueur3", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    Tour tourBlanche(Position(0, 0), Couleur::BLANC, &j1);
    j1.ajouterPiece(&tourBlanche);
    jeu.getPlateau().placerPiece(Position(0, 0), &tourBlanche);

    ControleurJeu controleur(&jeu);

    std::cout << "Avant clic :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;

    // 1er clic : sélectionner la pièce
    controleur.gererClicCase(Position(0, 0));

    // 2e clic : demander le déplacement
    controleur.gererClicCase(Position(1, 0));

    std::cout << std::endl;
    std::cout << "Apres clics :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;
    std::cout << "Historique : " << jeu.getHistoriqueCoups().size() << std::endl;

    return 0;
}