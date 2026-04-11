#include <iostream>
#include <vector>
#include "src/model/Plateau.h"
#include "src/model/Tour.h"
#include "src/model/JoueurHumain.h"

int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Tour tourBlanche(Position(3, 3), Couleur::BLANC, &j1);
    Tour tourNoire(Position(3, 6), Couleur::NOIR, &j2);
    Tour tourAlliee(Position(1, 3), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(3, 3), &tourBlanche);
    plateau.placerPiece(Position(3, 6), &tourNoire);
    plateau.placerPiece(Position(1, 3), &tourAlliee);

    std::vector<Position> mouvements = tourBlanche.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles de la tour blanche :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}