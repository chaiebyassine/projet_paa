#include <iostream>
#include "src/model/position/position.h"

int main() {
    Position p(2, 3);

    std::cout << "Ligne: " << p.getLigne() << std::endl;
    std::cout << "Colonne: " << p.getColonne() << std::endl;

    return 0;
}