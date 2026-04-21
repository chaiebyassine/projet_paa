#include "vueSFML.h"
#include "../model/plateau/case.h"
#include "../model/piece/piece.h"
#include "../model/joueur/joueur.h"
#include <cmath>
#include <iostream>
#include <climits>

// ===========================================================
// Geometrie hexagonale fidelement inspiree du plateau Yalta reel.
//
// Le plateau est un hexagone compose de 6 matrices de 16 losanges
// (4x4) chacune. Les 3 paires forment les zones des 3 joueurs.
//
// Mapping modele (ligne 0..11, col 0..7) -> matrice:
//   Zone BLANC : lignes  0..3  -> matrices 1(col 0..3) et 2(col 4..7)
//   Zone ROUGE : lignes  4..7  -> matrices 3(col 4..7) et 4(col 0..3)
//   Zone NOIR  : lignes  8..11 -> matrices 5(col 0..3) et 6(col 4..7)
// ===========================================================

static const float PI = 3.14159265359f;

static sf::Vector2f milieu(const sf::Vector2f& a, const sf::Vector2f& b) {
    return (a + b) / 2.f;
}

// Cree un losange (ConvexShape 4 points)
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

// Retourne le centre d'un losange (moyenne des diagonales)
static sf::Vector2f centreLosange(const sf::ConvexShape& s) {
    return (s.getPoint(0) + s.getPoint(2)) / 2.f;
}

// Verifie si un point est dans un ConvexShape (ray casting)
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

// ============================================================
// createMatrixLines : cree les lignes de division d'une matrice 4x4
// (meme logique que MakeBoard::createMatrixLines)
// ============================================================
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

// ============================================================
// createMatrixLosange : cree les 16 losanges d'une matrice 4x4
// (meme logique que MakeBoard::createMatrixLosange)
// ============================================================
static std::vector<sf::ConvexShape> createMatrixLosange(
    const sf::Vector2f& center,
    const std::vector<sf::Vector2f>& ml,
    const sf::Vector2f& point,
    const sf::Vector2f& mil1, const sf::Vector2f& mil2,
    sf::Color c1, sf::Color c2)
{
    std::vector<sf::ConvexShape> s;
    // Rangee 0 (pres du centre)
    s.push_back(makeLosange(center, ml[0], ml[6]+0.25f*(ml[7]-ml[6]), ml[6], c1));
    s.push_back(makeLosange(ml[0], ml[2], ml[6]+0.5f*(ml[7]-ml[6]), ml[6]+0.25f*(ml[7]-ml[6]), c2));
    s.push_back(makeLosange(ml[2], ml[4], ml[6]+0.75f*(ml[7]-ml[6]), ml[6]+0.5f*(ml[7]-ml[6]), c1));
    s.push_back(makeLosange(ml[4], mil1,  ml[7], ml[6]+0.75f*(ml[7]-ml[6]), c2));
    // Rangee 1
    s.push_back(makeLosange(ml[6], ml[6]+0.25f*(ml[7]-ml[6]), ml[8]+0.25f*(ml[9]-ml[8]), ml[8], c2));
    s.push_back(makeLosange(ml[6]+0.25f*(ml[7]-ml[6]), ml[6]+0.5f*(ml[7]-ml[6]), ml[8]+0.5f*(ml[9]-ml[8]), ml[8]+0.25f*(ml[9]-ml[8]), c1));
    s.push_back(makeLosange(ml[6]+0.5f*(ml[7]-ml[6]), ml[6]+0.75f*(ml[7]-ml[6]), ml[8]+0.75f*(ml[9]-ml[8]), ml[8]+0.5f*(ml[9]-ml[8]), c2));
    s.push_back(makeLosange(ml[6]+0.75f*(ml[7]-ml[6]), ml[7], ml[9], ml[8]+0.75f*(ml[9]-ml[8]), c1));
    // Rangee 2
    s.push_back(makeLosange(ml[8], ml[8]+0.25f*(ml[9]-ml[8]), ml[10]+0.25f*(ml[11]-ml[10]), ml[10], c1));
    s.push_back(makeLosange(ml[8]+0.25f*(ml[9]-ml[8]), ml[8]+0.5f*(ml[9]-ml[8]), ml[10]+0.5f*(ml[11]-ml[10]), ml[10]+0.25f*(ml[11]-ml[10]), c2));
    s.push_back(makeLosange(ml[8]+0.5f*(ml[9]-ml[8]), ml[8]+0.75f*(ml[9]-ml[8]), ml[10]+0.75f*(ml[11]-ml[10]), ml[10]+0.5f*(ml[11]-ml[10]), c1));
    s.push_back(makeLosange(ml[8]+0.75f*(ml[9]-ml[8]), ml[9], ml[11], ml[10]+0.75f*(ml[11]-ml[10]), c2));
    // Rangee 3 (bord exterieur = pieces)
    s.push_back(makeLosange(ml[10], ml[10]+0.25f*(ml[11]-ml[10]), ml[1],  mil2, c2));
    s.push_back(makeLosange(ml[10]+0.25f*(ml[11]-ml[10]), ml[10]+0.5f*(ml[11]-ml[10]), ml[3], ml[1], c1));
    s.push_back(makeLosange(ml[10]+0.5f*(ml[11]-ml[10]), ml[10]+0.75f*(ml[11]-ml[10]), ml[5], ml[3], c2));
    s.push_back(makeLosange(ml[10]+0.75f*(ml[11]-ml[10]), ml[5], point, ml[11], c1));
    return s;
}

// ============================================================
// Constructeur : construit la geometrie hexagonale complete
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

void VueSFML::buildBoard() {
    // --- Hexagone (meme parametres que MakeBoard) ---
    std::vector<float> sides  = {450,460,460,450,460,460};
    std::vector<float> sides2 = {500,510,510,500,510,510};

    sf::Vector2f origin(350.f, 150.f);

    // Points de l'hexagone principal
    std::vector<sf::Vector2f> pts;
    pts.push_back(origin + sf::Vector2f(sides[0], 0));
    float ang = 0;
    for (int i = 1; i < 6; i++) {
        ang += PI/3;
        pts.push_back(pts.back() + sf::Vector2f(std::cos(ang),std::sin(ang))*sides[i]);
    }

    // Centre de l'hexagone
    sf::Vector2f ctr(0,0);
    for (auto& p : pts) ctr += p;
    ctr /= 6.f;

    // Points hexagone 2
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

    // Hexagone de fond (affichage)
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

    // Milieux des aretes
    sf::Vector2f mils[6] = {
        milieu(pts[4],pts[3]), // 0 bas gauche
        milieu(pts[3],pts[2]), // 1 bas
        milieu(pts[0],pts[1]), // 2 haut droite
        milieu(pts[0],pts[5]), // 3 haut
        milieu(pts[5],pts[4]), // 4 haut gauche
        milieu(pts[1],pts[2])  // 5 bas droite
    };

    // Couleurs des zones
    sf::Color blanc (0xFE,0xF7,0xE5);
    sf::Color beige (0xEE,0xCF,0xA1);

    // Zone ROUGE (haut, matrices 1+2)
    sf::Color rougeCl(255,180,180), rougeFc(190,60,60);
    // Zone NOIR  (bas gauche, matrices 3+4)
    sf::Color noirCl (160,210,160), noirFc (30,110,30);
    // Zone BLANC (bas droite, matrices 5+6)
    sf::Color blancCl(180,200,255), blancFc(50,90,200);

    // Matrices 1..6 (comme MakeBoard)
    auto ml1 = createMatrixLines(ctr, mils[1], mils[0], pts[3], pts[4], pts[2]);
    auto ml2 = createMatrixLines(ctr, mils[5], mils[1], pts[2], pts[3], pts[1]);
    auto ml3 = createMatrixLines(ctr, mils[0], mils[4], pts[4], pts[5], pts[3]);
    auto ml4 = createMatrixLines(ctr, mils[4], mils[3], pts[5], pts[0], pts[4]);
    auto ml5 = createMatrixLines(ctr, mils[2], mils[5], pts[1], pts[2], pts[0]);
    auto ml6 = createMatrixLines(ctr, mils[3], mils[2], pts[0], pts[1], pts[5]);

    matrices[0] = createMatrixLosange(ctr, ml1, pts[3], mils[1], mils[0], rougeFc, rougeCl);
    matrices[1] = createMatrixLosange(ctr, ml2, pts[2], mils[5], mils[1], rougeCl, rougeFc);
    matrices[2] = createMatrixLosange(ctr, ml3, pts[4], mils[0], mils[4], noirCl,  noirFc );
    matrices[3] = createMatrixLosange(ctr, ml4, pts[5], mils[4], mils[3], noirFc,  noirCl );
    matrices[4] = createMatrixLosange(ctr, ml5, pts[1], mils[2], mils[5], blancFc, blancCl);
    matrices[5] = createMatrixLosange(ctr, ml6, pts[0], mils[3], mils[2], blancCl, blancFc);

    // Precalcul du mapping modele -> losange
    // Modele: 12 lignes x 8 cols
    // Ligne 0..3  = zone ROUGE  (mat 0+1), col 0..3 -> mat0, col 4..7 -> mat1
    // Ligne 4..7  = zone NOIR   (mat 2+3), col 0..3 -> mat3, col 4..7 -> mat2
    // Ligne 8..11 = zone BLANC  (mat 4+5), col 0..3 -> mat5, col 4..7 -> mat4
    // Dans chaque matrice: index 0..15, rangee 0=centre, 3=bord ext
    // Rangee locale r = 3 - (ligne % 4) pour rouge/blanc
    //                r = ligne % 4        pour noir
    // Col locale c  = col % 4
    // Index losange = r*4 + c
    for (int l = 0; l < 12; l++) {
        for (int c = 0; c < 8; c++) {
            caseVersLosange[l][c] = {-1, -1};
            int mat = -1; int idx = -1;
            int rc = c % 4; // colonne locale dans la matrice

            if (l < 4) {
                // Zone ROUGE : pieces sur lignes 0-1 (bord exterieur = rang 3)
                // rang 0 = proche centre (ligne 3), rang 3 = bord (ligne 0)
                int rl = 3 - l; // ligne 0 -> rang 3, ligne 3 -> rang 0
                if (c < 4) { mat = 0; idx = rl*4 + rc; }
                else        { mat = 1; idx = rl*4 + (3-rc); }
            } else if (l < 8) {
                // Zone NOIR : pieces sur lignes 10-11 (bord ext = rang 3)
                // mais dans le modele les lignes NOIR sont 4..7? Non:
                // initialiserPieces: NOIR = lignes 10-11
                // Donc l=4..7 est une zone neutre dans le modele courant
                // On les met quand meme quelque part (zones centrales)
                int rl = l - 4; // 0..3 rang local
                if (c < 4) { mat = 3; idx = rl*4 + rc; }
                else        { mat = 2; idx = rl*4 + (3-rc); }
            } else {
                // Zone NOIR (modele lignes 8-11) -> matrices 2+3
                // pieces sur ligne 10-11 = bord ext = rang 3
                int rl = l - 8; // 0..3
                if (c < 4) { mat = 5; idx = rl*4 + rc; }
                else        { mat = 4; idx = rl*4 + (3-rc); }
            }
            caseVersLosange[l][c] = {mat, idx};
        }
    }

    // Init table inverse a -1
    for (int m = 0; m < 6; m++)
        for (int i = 0; i < 16; i++)
            losangeVersCase[m][i] = {-1,-1};
    // Construit la table inverse: losange -> (ligne,col)
    for (int l = 0; l < 12; l++)
        for (int c = 0; c < 8; c++) {
            auto [m, i] = caseVersLosange[l][c];
            if (m >= 0) losangeVersCase[m][i] = {l,c};
        }
}

// ============================================================
// run() - boucle principale
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

    // Copie du losange en jaune
    sf::ConvexShape hl = matrices[ms][is];
    hl.setFillColor(sf::Color(255,220,40,210));
    window.draw(hl);

    // Destinations possibles
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
            Couleur col = p->getCouleur();

            sf::Color txtCol, outCol;
            if      (col == Couleur::BLANC) { txtCol = sf::Color(210,230,255); outCol = sf::Color(20,20,80);   }
            else if (col == Couleur::ROUGE) { txtCol = sf::Color(255,210,210); outCol = sf::Color(80,0,0);     }
            else                            { txtCol = sf::Color(30,30,30);    outCol = sf::Color(180,180,180);}

            auto [m, i] = caseVersLosange[l][c];
            if (m < 0) continue;
            sf::Vector2f cpos = centreLosange(matrices[m][i]);

            sf::Text txt(font, p->getSymbole(), 22u);
            txt.setStyle(sf::Text::Bold);
            txt.setFillColor(txtCol);
            txt.setOutlineColor(outCol);
            txt.setOutlineThickness(1.5f);
            auto bounds = txt.getLocalBounds();
            txt.setPosition({cpos.x - bounds.size.x*0.5f - bounds.position.x,
                             cpos.y - bounds.size.y*0.5f - bounds.position.y});
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
