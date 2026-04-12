#include <iostream>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"

int main() {
    Jeu jeu(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);
    JoueurHumain j3("Joueur3", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    
    jeu.demarrerPartie();
Case* c1 = jeu.getPlateau().obtenirCase(Position(0, 0));
Case* c2 = jeu.getPlateau().obtenirCase(Position(0, 1));
Case* c3 = jeu.getPlateau().obtenirCase(Position(7, 7));

std::cout << "Case (0,0) occupee ? " << c1->estOccupee() << std::endl;
std::cout << "Case (0,1) occupee ? " << c2->estOccupee() << std::endl;
std::cout << "Case (7,7) occupee ? " << c3->estOccupee() << std::endl;
    std::cout << "Joueur courant : " << jeu.getJoueurCourant()->getNom() << std::endl;

    return 0;
}