#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <array>
#include "../model/jeu.h"
#include "../model/base/position.h"
#include "../controller/controleurjeu.h"

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
    Jeu&             jeu;
    ControleurJeu    controleur;

    sf::ConvexShape hexBorder;
    sf::ConvexShape hexFill;

    std::vector<sf::ConvexShape> matrices[6];

    std::pair<int,int> caseVersLosange[12][8];
    std::pair<int,int> losangeVersCase[6][16];

    void buildBoard();
    void dessinerCases();
    void dessinerHighlights();
    void dessinerPieces();
    void dessinerInfo();
    Position sourisVersPosition(int mx, int my) const;
};