#include <iostream>
#include <vector>
#include "src/model/Plateau.h"
#include "src/model/Cavalier.h"
#include "src/model/Tour.h"
#include "src/model/JoueurHumain.h"

int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Cavalier cavalierBlanc(Position(3, 3), Couleur::BLANC, &j1);
    Tour pieceAdverse(Position(5, 4), Couleur::NOIR, &j2);
    Tour pieceAlliee(Position(1, 4), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(3, 3), &cavalierBlanc);
    plateau.placerPiece(Position(5, 4), &pieceAdverse);
    plateau.placerPiece(Position(1, 4), &pieceAlliee);

    std::vector<Position> mouvements = cavalierBlanc.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles du cavalier blanc :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}