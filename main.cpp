#include <iostream>
#include <vector>
#include "src/model/Plateau.h"
#include "src/model/Fou.h"
#include "src/model/JoueurHumain.h"
#include "src/model/Tour.h"
int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Fou fouBlanc(Position(3, 3), Couleur::BLANC, &j1);
    Tour pieceAdverse(Position(5, 5), Couleur::NOIR, &j2);
    Tour pieceAlliee(Position(1, 1), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(3, 3), &fouBlanc);
    plateau.placerPiece(Position(5, 5), &pieceAdverse);
    plateau.placerPiece(Position(1, 1), &pieceAlliee);

    std::vector<Position> mouvements = fouBlanc.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles du fou blanc :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}