#include <iostream>
#include "src/model/Jeu.h"
#include "src/model/joueur/JoueurHumain.h"
#include "src/model/piece/Tour.h"

int main() {
    Jeu jeu(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);
    JoueurHumain j3("Joueur3", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    Tour tourBlanche(Position(3, 3), Couleur::BLANC, &j1);

    jeu.getPlateau().placerPiece(Position(3, 3), &tourBlanche);

    std::cout << "Joueur courant : " << jeu.getJoueurCourant()->getNom() << std::endl;

    bool ok = jeu.deplacerPiece(Position(3, 3), Position(3, 6));

    std::cout << "Deplacement reussi ? " << ok << std::endl;
    std::cout << "Nouveau joueur courant : " << jeu.getJoueurCourant()->getNom() << std::endl;

    return 0;
}