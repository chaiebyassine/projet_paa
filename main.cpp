#include <iostream>
#include <vector>
#include "src/model/Plateau.h"
#include "src/model/Reine.h"
#include "src/model/Tour.h"
#include "src/model/JoueurHumain.h"

int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Reine reineBlanche(Position(3, 3), Couleur::BLANC, &j1);
    Tour pieceAdverse(Position(3, 6), Couleur::NOIR, &j2);
    Tour pieceAlliee(Position(1, 1), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(3, 3), &reineBlanche);
    plateau.placerPiece(Position(3, 6), &pieceAdverse);
    plateau.placerPiece(Position(1, 1), &pieceAlliee);

    std::vector<Position> mouvements = reineBlanche.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles de la reine blanche :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}