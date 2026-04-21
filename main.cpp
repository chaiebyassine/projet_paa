#include <iostream>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/model/piece/piece.h"
#include "src/controller/controleurjeu.h"

int main() {

    // Plateau Yalta : 12 lignes x 8 colonnes = 96 cases
    Jeu jeu(12, 8);

    JoueurHumain j1("Blanc", Couleur::BLANC);
    JoueurHumain j2("Noir",  Couleur::NOIR);
    JoueurHumain j3("Rouge", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    // Place les 48 pieces (16 par joueur) dans leurs zones respectives
    jeu.initialiserPieces();

    // Compte les pieces placees pour verifier
    int nbPieces = 0;
    for (int l = 0; l < 12; l++)
        for (int c = 0; c < 8; c++)
            if (jeu.getPlateau().obtenirCase(Position(l, c))->estOccupee())
                nbPieces++;
    std::cout << "Pieces sur le plateau : " << nbPieces << " / 48" << std::endl;

    // Affichage du plateau : B=Blanc  N=Noir  R=Rouge  .=vide
    std::cout << "\n   0 1 2 3 4 5 6 7" << std::endl;
    for (int l = 0; l < 12; l++) {
        if (l < 10) std::cout << " ";
        std::cout << l << " ";

        for (int c = 0; c < 8; c++) {
            const Case* ca = jeu.getPlateau().obtenirCase(Position(l, c));
            if (ca->estOccupee()) {
                Couleur col = ca->getPiece()->getCouleur();
                if      (col == Couleur::BLANC) std::cout << "B ";
                else if (col == Couleur::NOIR)  std::cout << "N ";
                else                            std::cout << "R ";
            } else {
                std::cout << ". ";
            }
        }

        if      (l == 0)  std::cout << "<-- Blanc : pieces";
        else if (l == 1)  std::cout << "<-- Blanc : pions (avancent vers ligne 11)";
        else if (l == 5)  std::cout << "<-- Rouge : pions (avancent vers ligne 11)";
        else if (l == 6)  std::cout << "<-- Rouge : pieces";
        else if (l == 10) std::cout << "<-- Noir  : pions (avancent vers ligne 0)";
        else if (l == 11) std::cout << "<-- Noir  : pieces";
        std::cout << std::endl;
    }

    // Test : deplacer un pion blanc de (1,0) vers (2,0)
    ControleurJeu controleur(&jeu);
    std::cout << "\nTest : pion blanc (1,0) -> (2,0)" << std::endl;
    controleur.gererClicCase(Position(1, 0));
    controleur.gererClicCase(Position(2, 0));
    std::cout << "Case (1,0) occupee : " << jeu.getPlateau().obtenirCase(Position(1,0))->estOccupee() << std::endl;
    std::cout << "Case (2,0) occupee : " << jeu.getPlateau().obtenirCase(Position(2,0))->estOccupee() << std::endl;
    std::cout << "Historique         : " << jeu.getHistoriqueCoups().size() << " coup(s)" << std::endl;

    return 0;
}
