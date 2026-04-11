#include <iostream>
#include <vector>
#include "src/model/plateau/Plateau.h"
#include "src/model/piece/Pion.h"
#include "src/model/piece/Tour.h"
#include "src/model/joueur/JoueurHumain.h"

int main() {
    Plateau plateau(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);

    Pion pionBlanc(Position(6, 3), Couleur::BLANC, &j1);
    Tour pieceAdverse(Position(5, 4), Couleur::NOIR, &j2);
    Tour pieceAlliee(Position(5, 2), Couleur::BLANC, &j1);

    plateau.placerPiece(Position(6, 3), &pionBlanc);
    plateau.placerPiece(Position(5, 4), &pieceAdverse);
    plateau.placerPiece(Position(5, 2), &pieceAlliee);

    std::vector<Position> mouvements = pionBlanc.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles du pion blanc :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}