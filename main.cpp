#include <iostream>
#include "src/model/Position.h"
#include "src/model/Case.h"
#include "src/model/Plateau.h"

int main() {
    Plateau plateau(8, 8);

    Position p1(2, 3);
    Position p2(4, 5);

    std::cout << "Case (2,3) valide ? " << plateau.estCaseValide(p1) << std::endl;
    std::cout << "Case (4,5) valide ? " << plateau.estCaseValide(p2) << std::endl;

    return 0;
}