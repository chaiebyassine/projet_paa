#include "vueSFML.h"
#include "../model/plateau/case.h"
#include "../model/piece/piece.h"
#include "../model/joueur/joueur.h"
#include <cmath>
#include <iostream>
#include <climits>

static const float PI = 3.14159265359f;

static sf::Vector2f milieu(const sf::Vector2f& a, const sf::Vector2f& b) {
    return (a + b) / 2.f;
}

static sf::ConvexShape makeLosange(sf::Vector2f p0, sf::Vector2f p1,
                                    sf::Vector2f p2, sf::Vector2f p3,
                                    sf::Color fill) {
    sf::ConvexShape s(4);
    s.setPoint(0, p0); s.setPoint(1, p1);
    s.setPoint(2, p2); s.setPoint(3, p3);
    s.setFillColor(fill);
    s.setOutlineColor(sf::Color::Black);
    s.setOutlineThickness(1.5f);
    return s;
}

static sf::Vector2f centreLosange(const sf::ConvexShape& s) {
    return (s.getPoint(0) + s.getPoint(2)) / 2.f;
}

static bool pointDansLosange(const sf::ConvexShape& s, sf::Vector2f pt) {
    int n = (int)s.getPointCount();
    bool inside = false;
    for (int i = 0, j = n-1; i < n; j = i++) {
        sf::Vector2f pi = s.getPoint(i), pj = s.getPoint(j);
        if (((pi.y > pt.y) != (pj.y > pt.y)) &&
            (pt.x < (pj.x-pi.x)*(pt.y-pi.y)/(pj.y-pi.y)+pi.x))
            inside = !inside;
    }
    return inside;
}

static std::vector<sf::Vector2f> createMatrixLines(
    const sf::Vector2f& center,
    const sf::Vector2f& mil1, const sf::Vector2f& mil2,
    const sf::Vector2f& pt1,  const sf::Vector2f& pt2, const sf::Vector2f& pt3)
{
    std::vector<sf::Vector2f> lines;
    for (float r : {0.25f, 0.50f, 0.75f}) {
        lines.push_back(center + r*(mil1 - center));
        lines.push_back((1-(1-r)/2)*pt1 + ((1-r)/2)*pt2);
    }
    for (float r : {0.25f, 0.50f, 0.75f}) {
        lines.push_back(center + r*(mil2 - center));
        lines.push_back((1-(1+r)/2)*pt3 + ((1+r)/2)*pt1);
    }
    return lines;
}

static std::vector<sf::ConvexShape> createMatrixLosange(
    const sf::Vector2f& center,
    const std::vector<sf::Vector2f>& ml,
    const sf::Vector2f& point,
    const sf::Vector2f& mil1, const sf::Vector2f& mil2,
    sf::Color c1, sf::Color c2)
{
    std::vector<sf::ConvexShape> s;
    // Rang 0 (centre) -> idx 0..3
    s.push_back(makeLosange(center, ml[0], ml[6]+0.25f*(ml[7]-ml[6]), ml[6], c1));
    s.push_back(makeLosange(ml[0], ml[2], ml[6]+0.5f*(ml[7]-ml[6]), ml[6]+0.25f*(ml[7]-ml[6]), c2));
    s.push_back(makeLosange(ml[2], ml[4], ml[6]+0.75f*(ml[7]-ml[6]), ml[6]+0.5f*(ml[7]-ml[6]), c1));
    s.push_back(makeLosange(ml[4], mil1,  ml[7], ml[6]+0.75f*(ml[7]-ml[6]), c2));
    // Rang 1 -> idx 4..7
    s.push_back(makeLosange(ml[6], ml[6]+0.25f*(ml[7]-ml[6]), ml[8]+0.25f*(ml[9]-ml[8]), ml[8], c2));
    s.push_back(makeLosange(ml[6]+0.25f*(ml[7]-ml[6]), ml[6]+0.5f*(ml[7]-ml[6]), ml[8]+0.5f*(ml[9]-ml[8]), ml[8]+0.25f*(ml[9]-ml[8]), c1));
    s.push_back(makeLosange(ml[6]+0.5f*(ml[7]-ml[6]), ml[6]+0.75f*(ml[7]-ml[6]), ml[8]+0.75f*(ml[9]-ml[8]), ml[8]+0.5f*(ml[9]-ml[8]), c2));
    s.push_back(makeLosange(ml[6]+0.75f*(ml[7]-ml[6]), ml[7], ml[9], ml[8]+0.75f*(ml[9]-ml[8]), c1));
    // Rang 2 -> idx 8..11
    s.push_back(makeLosange(ml[8], ml[8]+0.25f*(ml[9]-ml[8]), ml[10]+0.25f*(ml[11]-ml[10]), ml[10], c1));
    s.push_back(makeLosange(ml[8]+0.25f*(ml[9]-ml[8]), ml[8]+0.5f*(ml[9]-ml[8]), ml[10]+0.5f*(ml[11]-ml[10]), ml[10]+0.25f*(ml[11]-ml[10]), c2));
    s.push_back(makeLosange(ml[8]+0.5f*(ml[9]-ml[8]), ml[8]+0.75f*(ml[9]-ml[8]), ml[10]+0.75f*(ml[11]-ml[10]), ml[10]+0.5f*(ml[11]-ml[10]), c1));
    s.push_back(makeLosange(ml[8]+0.75f*(ml[9]-ml[8]), ml[9], ml[11], ml[10]+0.75f*(ml[11]-ml[10]), c2));
    // Rang 3 (bord) -> idx 12..15
    s.push_back(makeLosange(ml[10], ml[10]+0.25f*(ml[11]-ml[10]), ml[1],  mil2, c2));
    s.push_back(makeLosange(ml[10]+0.25f*(ml[11]-ml[10]), ml[10]+0.5f*(ml[11]-ml[10]), ml[3], ml[1], c1));
    s.push_back(makeLosange(ml[10]+0.5f*(ml[11]-ml[10]), ml[10]+0.75f*(ml[11]-ml[10]), ml[5], ml[3], c2));
    s.push_back(makeLosange(ml[10]+0.75f*(ml[11]-ml[10]), ml[5], point, ml[11], c1));
    return s;
}

// ============================================================
// Constructeur
// ============================================================
VueSFML::VueSFML(Jeu& jeu)
    : jeu(jeu), controleur(&jeu),
      window(sf::VideoMode({WIN_W, WIN_H}), "Yalta Chess",
             sf::Style::Titlebar | sf::Style::Close)
{
    window.setFramerateLimit(60);
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        (void)font.openFromFile("C:\\Windows\\Fonts\\consola.ttf");

    buildBoard();
}

// ============================================================
// buildBoard
// ============================================================
void VueSFML::buildBoard() {
    std::vector<float> sides  = {450,460,460,450,460,460};
    std::vector<float> sides2 = {500,510,510,500,510,510};

    sf::Vector2f origin(350.f, 150.f);

    std::vector<sf::Vector2f> pts;
    pts.push_back(origin + sf::Vector2f(sides[0], 0));
    float ang = 0;
    for (int i = 1; i < 6; i++) {
        ang += PI/3;
        pts.push_back(pts.back() + sf::Vector2f(std::cos(ang),std::sin(ang))*sides[i]);
    }

    sf::Vector2f ctr(0,0);
    for (auto& p : pts) ctr += p;
    ctr /= 6.f;

    std::vector<sf::Vector2f> pts2;
    pts2.push_back(origin + sf::Vector2f(sides2[0], 0));
    ang = 0;
    for (int i = 1; i < 6; i++) {
        ang += PI/3;
        pts2.push_back(pts2.back() + sf::Vector2f(std::cos(ang),std::sin(ang))*sides2[i]);
    }
    sf::Vector2f ctr2(0,0);
    for (auto& p : pts2) ctr2 += p;
    ctr2 /= 6.f;
    sf::Vector2f off = ctr - ctr2;
    for (auto& p : pts2) p += off;

    hexBorder.setPointCount(6);
    for (int i = 0; i < 6; i++) hexBorder.setPoint(i, pts[i]);
    hexBorder.setFillColor(sf::Color::Transparent);
    hexBorder.setOutlineColor(sf::Color::White);
    hexBorder.setOutlineThickness(2);

    hexFill.setPointCount(6);
    for (int i = 0; i < 6; i++) hexFill.setPoint(i, pts2[i]);
    hexFill.setFillColor(sf::Color(48,46,43));
    hexFill.setOutlineColor(sf::Color::Black);
    hexFill.setOutlineThickness(7);

    sf::Vector2f mils[6] = {
        milieu(pts[4],pts[3]),
        milieu(pts[3],pts[2]),
        milieu(pts[0],pts[1]),
        milieu(pts[0],pts[5]),
        milieu(pts[5],pts[4]),
        milieu(pts[1],pts[2])
    };

    // Couleurs
    sf::Color blancCl(180,200,255), blancFc(50,90,200);
    sf::Color rougeCl(255,180,180), rougeFc(190,60,60);
    sf::Color noirCl (160,210,160), noirFc (30,110,30);

    auto ml1 = createMatrixLines(ctr, mils[1], mils[0], pts[3], pts[4], pts[2]);
    auto ml2 = createMatrixLines(ctr, mils[5], mils[1], pts[2], pts[3], pts[1]);
    auto ml3 = createMatrixLines(ctr, mils[0], mils[4], pts[4], pts[5], pts[3]);
    auto ml4 = createMatrixLines(ctr, mils[4], mils[3], pts[5], pts[0], pts[4]);
    auto ml5 = createMatrixLines(ctr, mils[2], mils[5], pts[1], pts[2], pts[0]);
    auto ml6 = createMatrixLines(ctr, mils[3], mils[2], pts[0], pts[1], pts[5]);

    // matrices 0-1 = BLANC (bleu)
    // matrices 2-3 = ROUGE (rouge)
    // matrices 4-5 = NOIR  (vert)
    matrices[0] = createMatrixLosange(ctr, ml1, pts[3], mils[1], mils[0], blancFc, blancCl);
    matrices[1] = createMatrixLosange(ctr, ml2, pts[2], mils[5], mils[1], blancCl, blancFc);
    matrices[2] = createMatrixLosange(ctr, ml3, pts[4], mils[0], mils[4], rougeCl, rougeFc);
    matrices[3] = createMatrixLosange(ctr, ml4, pts[5], mils[4], mils[3], rougeFc, rougeCl);
    matrices[4] = createMatrixLosange(ctr, ml5, pts[1], mils[2], mils[5], noirFc,  noirCl );
    matrices[5] = createMatrixLosange(ctr, ml6, pts[0], mils[3], mils[2], noirCl,  noirFc );

    // ============================================================
    // Mapping modele -> losange
    //
    // idx = rc + 4 * rl
    //   rc = c % 4  (colonne locale 0..3)
    //   rl = rang   (0=centre, 3=bord)
    //
    // BLANC (l=0..3) : mat=(c<4)?0:1,  rl=3-l
    // ROUGE (l=4..7) : mat=(c<4)?2:3,  rl=l-4
    // NOIR (l=8..11) : mat=(c<4)?4:5,  rl=l-8
 for (int l = 0; l < 12; l++) {
    for (int c = 0; c < 8; c++) {
        int rc = c % 4;
        int mat = -1;
        int rl  = -1;
        int idx = -1;

        if (l < 4) {
            // BLANC -> bas
            rl = 3 - l;

            if (c < 4) {
                mat = 0;
                idx = (3 - rc) * 4 + rl;
            } else {
                mat = 1;
                idx = rc + 4 * rl;
            }
        }
       else if (l < 8) {
    // ROUGE -> gauche
    rl = l - 4;

    if (c < 4) {
        // moitié gauche
        mat = 2;
        idx = rc + 4 * rl;
    } else {
        // moitié droite (CONTINUE dans le même sens)
        mat = 3;
        idx = (c - 4) + 4 * rl;
    }
}
        else {
            // NOIR -> droite
            rl = l - 8;

            if (c < 4) {
                mat = 4;
                idx = (3 - rc) * 4 + rl;
            } else {
                mat = 5;
                idx = rc + 4 * rl;
            }
        }

        caseVersLosange[l][c] = {mat, idx};
    }
}
    // Table inverse
    for (int m = 0; m < 6; m++)
        for (int i = 0; i < 16; i++)
            losangeVersCase[m][i] = {-1,-1};

    for (int l = 0; l < 12; l++)
        for (int c = 0; c < 8; c++) {
            auto [m, i] = caseVersLosange[l][c];
            if (m >= 0 && i >= 0 && i < 16)
                losangeVersCase[m][i] = {l,c};
        }
}

// ============================================================
// run()
// ============================================================
void VueSFML::run() {
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (const auto* mb = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    Position pos = sourisVersPosition(mb->position.x, mb->position.y);
                    if (pos.getLigne() >= 0)
                        controleur.gererClicCase(pos);
                }
            }
        }
        window.clear(sf::Color(48,46,43));
        dessinerCases();
        dessinerHighlights();
        dessinerPieces();
        dessinerInfo();
        window.display();
    }
}

// ============================================================
// Dessin des cases
// ============================================================
void VueSFML::dessinerCases() {
    window.draw(hexFill);
    for (int m = 0; m < 6; m++)
        for (auto& losange : matrices[m])
            window.draw(losange);
    window.draw(hexBorder);
}

// ============================================================
// Surbrillance
// ============================================================
void VueSFML::dessinerHighlights() {
    if (!controleur.aPieceSelectionnee()) return;
    Position sel = controleur.getPositionSelectionnee();

    auto [ms, is] = caseVersLosange[sel.getLigne()][sel.getColonne()];
    if (ms < 0) return;

    sf::ConvexShape hl = matrices[ms][is];
    hl.setFillColor(sf::Color(255,220,40,210));
    window.draw(hl);

    const Case* ca = jeu.getPlateau().obtenirCase(sel);
    if (ca && ca->estOccupee()) {
        auto moves = ca->getPiece()->mouvementsPossibles(jeu.getPlateau());
        for (const auto& mv : moves) {
            auto [md, id] = caseVersLosange[mv.getLigne()][mv.getColonne()];
            if (md < 0) continue;
            sf::ConvexShape dst = matrices[md][id];
            dst.setFillColor(sf::Color(50,200,50,170));
            window.draw(dst);
        }
    }
}

// ============================================================
// Pieces
// ============================================================
void VueSFML::dessinerPieces() {
    for (int l = 0; l < 12; l++) {
        for (int c = 0; c < 8; c++) {

            const Case* ca = jeu.getPlateau().obtenirCase(Position(l,c));
            if (!ca || !ca->estOccupee()) continue;

            const Piece* p = ca->getPiece();

            // 🔥 prendre seulement la première lettre
            std::string symbole = p->getSymbole();
            std::string lettre(1, symbole[0]); // EX: "P", "T", etc.

            // couleur selon joueur
            sf::Color txtCol;
            if (p->getCouleur() == Couleur::BLANC)
                txtCol = sf::Color::White;
            else if (p->getCouleur() == Couleur::ROUGE)
                txtCol = sf::Color::Red;
            else
                txtCol = sf::Color::Black;

            auto [m, i] = caseVersLosange[l][c];
            if (m < 0) continue;

            sf::Vector2f centre = centreLosange(matrices[m][i]);

            sf::Text txt(font, lettre, 20u);
            txt.setFillColor(txtCol);
            txt.setStyle(sf::Text::Bold);

            auto bounds = txt.getLocalBounds();
            txt.setPosition({
                centre.x - bounds.size.x / 2.f,
                centre.y - bounds.size.y / 2.f
            });

            window.draw(txt);
        }
    }
}

// ============================================================
// Info
// ============================================================
void VueSFML::dessinerInfo() {
    float py = (float)(WIN_H - INFO_H);
    sf::RectangleShape panel({(float)WIN_W, (float)INFO_H});
    panel.setPosition({0,py});
    panel.setFillColor(sf::Color(30,30,30));
    window.draw(panel);

    Joueur* j = jeu.getJoueurCourant();
    if (j) {
        Couleur c = j->getCouleur();
        sf::Color ct;
        std::string cn;
        if      (c==Couleur::BLANC){cn="Blanc";ct=sf::Color(180,200,255);}
        else if (c==Couleur::ROUGE){cn="Rouge";ct=sf::Color(255,130,130);}
        else                       {cn="Noir"; ct=sf::Color(130,220,130);}
        sf::Text t(font,"Tour : "+cn+" ("+j->getNom()+")",17u);
        t.setFillColor(ct);
        t.setPosition({12.f,py+10});
        window.draw(t);
    }
    sf::Text t2(font,"Clic: selectionner/deplacer",12u);
    t2.setFillColor(sf::Color(100,100,100));
    t2.setPosition({12.f,py+35});
    window.draw(t2);
}

// ============================================================
// Detection clic -> Position modele
// ============================================================
Position VueSFML::sourisVersPosition(int mx, int my) const {
    sf::Vector2f pt((float)mx,(float)my);
    for (int m = 0; m < 6; m++) {
        for (int i = 0; i < 16; i++) {
            if (pointDansLosange(matrices[m][i], pt)) {
                auto [l,c] = losangeVersCase[m][i];
                if (l >= 0) return Position(l,c);
            }
        }
    }
    return Position(-1,-1);
}