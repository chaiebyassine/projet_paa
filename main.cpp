#include <iostream>
#include <string>
#include "src/model/jeu.h"
#include "src/model/joueur/joueurHumain.h"
#include "src/model/plateau/case.h"
#include "src/model/piece/tour.h"

std::string couleurToString(Couleur couleur) {
    if (couleur == Couleur::BLANC) return "BLANC";
    if (couleur == Couleur::NOIR) return "NOIR";
    return "ROUGE";
}

void afficherInfosCase(Jeu& jeu, const Position& position) {
    Case* c = jeu.getPlateau().obtenirCase(position);

    if (c == nullptr) {
        std::cout << "Case invalide." << std::endl;
        return;
    }

    std::cout << "Case (" << position.getLigne() << ", " << position.getColonne() << ") : ";

    if (!c->estOccupee()) {
        std::cout << "vide" << std::endl;
        return;
    }

    Piece* piece = c->getPiece();
    std::cout << "occupee par une piece de couleur "
              << couleurToString(piece->getCouleur())
              << ", appartenant a "
              << piece->getJoueur()->getNom()
              << std::endl;
}

int main() {
    Jeu jeu(8, 8);

    JoueurHumain j1("Joueur1", Couleur::BLANC);
    JoueurHumain j2("Joueur2", Couleur::NOIR);
    JoueurHumain j3("Joueur3", Couleur::ROUGE);

    jeu.ajouterJoueur(&j1);
    jeu.ajouterJoueur(&j2);
    jeu.ajouterJoueur(&j3);

    // On place manuellement 2 pièces pour tester une capture
    Tour tourBlanche(Position(0, 0), Couleur::BLANC, &j1);
    Tour tourNoire(Position(0, 3), Couleur::NOIR, &j2);

    j1.ajouterPiece(&tourBlanche);
    j2.ajouterPiece(&tourNoire);

    jeu.getPlateau().placerPiece(Position(0, 0), &tourBlanche);
    jeu.getPlateau().placerPiece(Position(0, 3), &tourNoire);

    std::cout << "===== AVANT CAPTURE =====" << std::endl;
    std::cout << "Pieces joueur 1 : " << j1.getPieces().size() << std::endl;
    std::cout << "Pieces joueur 2 : " << j2.getPieces().size() << std::endl;
    afficherInfosCase(jeu, Position(0, 0));
    afficherInfosCase(jeu, Position(0, 3));

    bool captureReussie = jeu.deplacerPiece(Position(0, 0), Position(0, 3));

    std::cout << std::endl;
    std::cout << "Capture reussie ? " << captureReussie << std::endl;

    std::cout << std::endl;
    std::cout << "===== APRES CAPTURE =====" << std::endl;
    std::cout << "Pieces joueur 1 : " << j1.getPieces().size() << std::endl;
    std::cout << "Pieces joueur 2 : " << j2.getPieces().size() << std::endl;
    afficherInfosCase(jeu, Position(0, 0));
    afficherInfosCase(jeu, Position(0, 3));
std::cout << "Joueur1 en echec ? " << jeu.estEnEchec(&j1) << std::endl;
std::cout << "Joueur2 en echec ? " << jeu.estEnEchec(&j2) << std::endl;
std::cout << "Joueur3 en echec ? " << jeu.estEnEchec(&j3) << std::endl;
    return 0;
}