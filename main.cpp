#include <iostream>
#include "src/model/Position.h"
#include "src/model/Case.h"

int main() {
    Position p(2, 3);
    Case c(p);

    std::cout << "Case occupee ? " << c.estOccupee() << std::endl;

    return 0;
}