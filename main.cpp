#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/view/vueSFML.h"

int main() {

    // Plateau Yalta : 12 lignes x 8 colonnes = 96 cases
    Jeu jeu(12, 8);

    JoueurHumain j1("Alice", Couleur::BLANC);
    JoueurHumain j2("Bob",   Couleur::NOIR);
    JoueurHumain j3("Carol", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    // Place les 48 pieces (16 par joueur) dans leurs zones
    jeu.initialiserPieces();

    // Lance la fenetre SFML
    VueSFML vue(jeu);
    vue.run();

    return 0;
}
