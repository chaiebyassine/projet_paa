#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <array>
#include "../model/jeu.h"
#include "../model/base/position.h"
#include "../controller/controleurjeu.h"

// Vue SFML - plateau Yalta hexagonal reel.
// 6 matrices de 16 losanges = 96 cases (3 zones x 2 matrices).
class VueSFML {
public:
    static const int INFO_H = 70;
    static const unsigned WIN_W = 1150;
    static const unsigned WIN_H = 1000 + INFO_H;

    VueSFML(Jeu& jeu);
    void run();

private:
    sf::RenderWindow window;
    sf::Font         font;
    Jeu&          jeu;
    ControleurJeu controleur;

    // Hexagones de fond
    sf::ConvexShape hexBorder;
    sf::ConvexShape hexFill;

    // 6 matrices de 16 losanges chacune
    std::vector<sf::ConvexShape> matrices[6];

    // Mapping modele (ligne,col) -> {matrice, index_losange}
    std::pair<int,int> caseVersLosange[12][8];

    // Mapping inverse losangeVersCase[mat][idx] = {ligne, col} (-1 si invalide)
    // On stocke comme paire d'entiers pour eviter le probleme de constructeur de Position
    std::pair<int,int> losangeVersCase[6][16];

    void buildBoard();
    void dessinerCases();
    void dessinerHighlights();
    void dessinerPieces();
    void dessinerInfo();
    Position sourisVersPosition(int mx, int my) const;
};
