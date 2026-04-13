#include <iostream>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/model/piece/tour.h"
#include "src/command/commandedeplacement.h"

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

    std::cout << "Avant commande :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;

    CommandeDeplacement* cmd = new CommandeDeplacement(
        &jeu,
        &tourBlanche,
        Position(0, 0),
        Position(1, 0)
    );

    cmd->executer();
    jeu.ajouterCommandeHistorique(cmd);

    std::cout << std::endl;
    std::cout << "Apres execution :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;
    std::cout << "Historique : " << jeu.getHistoriqueCoups().size() << std::endl;

    cmd->annuler();

    std::cout << std::endl;
    std::cout << "Apres annulation :" << std::endl;
    std::cout << "Case (0,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(0, 0))->estOccupee() << std::endl;
    std::cout << "Case (1,0) occupee ? "
              << jeu.getPlateau().obtenirCase(Position(1, 0))->estOccupee() << std::endl;

    return 0;
}