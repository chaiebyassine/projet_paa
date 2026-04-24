#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/view/vueSFML.h"

int main() {

    // Plateau hexagonal Yalta : 6 sections + zone centrale = 91 cases
    Jeu jeu;

    JoueurHumain j1("Alice", Couleur::BLANC);
    JoueurHumain j2("Bob",   Couleur::ROUGE);
    JoueurHumain j3("Carol", Couleur::NOIR);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    // Place les pièces dans leurs zones de départ (rangée 3 = dos, rangée 2 = pions)
    jeu.initialiserPieces();

    // Lance la fenêtre SFML
    VueSFML vue(jeu);
    vue.run();

    return 0;
}

