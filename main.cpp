#include <iostream>
#include <vector>
#include "src/model/Position.h"
#include "src/model/Case.h"
#include "src/model/Plateau.h"
#include "src/model/Couleur.h"
#include "src/model/Tour.h"

int main() {
    Plateau plateau(8, 8);

    Tour tour(Position(3, 3), Couleur::BLANC, nullptr);

    std::vector<Position> mouvements = tour.mouvementsPossibles(plateau);

    std::cout << "Mouvements possibles de la tour :" << std::endl;
    for (const Position& p : mouvements) {
        std::cout << "(" << p.getLigne() << ", " << p.getColonne() << ")" << std::endl;
    }

    return 0;
}