#include <iostream>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/model/piece/tour.h"
#include "src/command/commandedeplacement.h"

// Point d'entrée du programme : teste le déplacement et l'annulation d'un coup
int main() {
    // Crée un jeu d'échecs avec un plateau 8x8
    Jeu jeu(8, 8);

    // Crée 3 joueurs humains avec des couleurs différentes
    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);
    JoueurHumain j3("Joueur3", Couleur::ROUGE);

    // Ajoute les 3 joueurs au jeu
    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    // Crée une tour blanche et la place sur le plateau en (0,0)
    Tour tourBlanche(Position(0, 0), Couleur::BLANC, &j1);
    j1.ajouterPiece(&tourBlanche);
    jeu.getPlateau().placerPiece(Position(0, 0), &tourBlanche);

    // Affiche l'état des cases avant le déplacement
    std::cout << "Avant commande :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;

    // Crée une commande de déplacement : tour de (0,0) vers (1,0)
    CommandeDeplacement* cmd = new CommandeDeplacement(
        &jeu,
        &tourBlanche,
        Position(0, 0),
        Position(1, 0)
    );

    // Exécute la commande de déplacement
    cmd->executer();
    // Ajoute la commande à l'historique des coups
    jeu.ajouterCommandeHistorique(cmd);

    // Affiche l'état des cases après le déplacement
    std::cout << std::endl;
    std::cout << "Apres execution :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;
    std::cout << "Historique : " << jeu.getHistoriqueCoups().size() << std::endl;

    // Annule la commande : remet la tour à sa position initiale
    cmd->annuler();

    // Affiche l'état des cases après l'annulation
    std::cout << std::endl;
    std::cout << "Apres annulation :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;

    return 0;
}