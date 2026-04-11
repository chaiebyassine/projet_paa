#include <iostream>
#include <vector>
#include "src/model/Plateau.h"
#include "src/model/roi.h"
#include "src/model/Tour.h"
#include "src/model/JoueurHumain.h"

int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Roi roiBlanc(Position(3, 3), Couleur::BLANC, &j1);
    Tour pieceAdverse(Position(4, 4), Couleur::NOIR, &j2);
    Tour pieceAlliee(Position(2, 3), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(3, 3), &roiBlanc);
    plateau.placerPiece(Position(4, 4), &pieceAdverse);
    plateau.placerPiece(Position(2, 3), &pieceAlliee);

    std::vector<Position> mouvements = roiBlanc.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles du roi blanc :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}