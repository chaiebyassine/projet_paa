#include <iostream>
#include "src/model/JoueurHumain.h"
#include "src/model/JoueurIA.h"

int main() {
    JoueurHumain j1("Alice", Couleur::BLANC);
    JoueurIA j2("Bot", Couleur::NOIR, 3);
    JoueurHumain j3("Bob", Couleur::ROUGE);
    j1.jouer();
    j2.jouer();
    j3.jouer();
    return 0;
}