#include "vueSFML.h"
#include "../model/plateau/case.h"
#include "../model/piece/piece.h"
#include "../model/joueur/joueur.h"
#include <cmath>
#include <iostream>
#include <stdexcept>

static const float PI_F = 3.14159265359f;

// ── Constructeur ──────────────────────────────────────────────
VueSFML::VueSFML(Jeu& jeu)
    : jeu(jeu), controleur(&jeu),
      window(sf::VideoMode({WIN_W, WIN_H}),
             "Yalta Chess - 3 joueurs",
             sf::Style::Titlebar | sf::Style::Close),
      textGame(font, sf::String(), 22u),
      textEchec(font, sf::String(), 22u),
      backButtonText(font, sf::String(), 20u)
{
    window.setFramerateLimit(60);

    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf"))
        (void)font.openFromFile("C:\\Windows\\Fonts\\consola.ttf");

    textGame  = sf::Text(font, "Yalta Chess", 22u);
    textEchec = sf::Text(font, "", 22u);

    chargerTextures();
    buildBoard();
    buildMapping();

    // S'enregistre comme observateur : Jeu appellera mettreAJour() après chaque coup
    jeu.ajouterObservateur(this);

    sf::Color beige(0xEE, 0xCF, 0xA1);
    backButton.setSize({100.f, 40.f});
    backButton.setPosition({float(WIN_W) - 115.f, 5.f});
    backButton.setFillColor(beige);
    backButton.setOutlineColor(sf::Color::Black);
    backButton.setOutlineThickness(2.f);
    backButtonText = sf::Text(font, "Accueil", 20u);
    backButtonText.setFillColor(sf::Color::Black);
    backButtonText.setPosition({float(WIN_W) - 110.f, 12.f});
}

// ── Textures des pièces ──────────────────────────────────────
void VueSFML::chargerTextures() {
    auto load = [&](const std::string& key, const std::string& file) {
        if (!textures[key].loadFromFile("image/" + file))
            std::cerr << "Impossible de charger : image/" << file << "\n";
    };
    for (const std::string& col : std::vector<std::string>{"White","Red","Black"}) {
        load(col + "Pawn",   col + "Pawn.png");
        load(col + "Rook",   col + "Rook.png");
        load(col + "Knight", col + "Knight.png");
        load(col + "Bishop", col + "Bishop.png");
        load(col + "Queen",  col + "Queen.png");
        load(col + "King",   col + "King.png");
    }
}

std::string VueSFML::getCleTexture(const Piece* p) const {
    std::string col;
    if      (p->getCouleur() == Couleur::BLANC) col = "White";
    else if (p->getCouleur() == Couleur::ROUGE) col = "Red";
    else                                         col = "Black";
    const std::string& s = p->getSymbole();
    if (s == "P") return col + "Pawn";
    if (s == "T") return col + "Rook";
    if (s == "C") return col + "Knight";
    if (s == "F") return col + "Bishop";
    if (s == "D") return col + "Queen";
    if (s == "R") return col + "King";
    return "";
}

// ── buildBoard — dessine hexagone + 6 matrices × 16 losanges + labels ──
void VueSFML::buildBoard() {
    // Grand hexagone (blanc au fond noir)
    std::vector<float> side_lengths = {450, 460, 460, 450, 460, 460};
    std::vector<sf::Vector2f> points;
    float angle = 0;
    sf::Vector2f origin(350, 150);
    points.push_back(origin + sf::Vector2f(side_lengths[0], 0));
    for (int i = 1; i < 6; i++) {
        angle += PI_F / 3;
        points.push_back(points.back() +
            sf::Vector2f(std::cos(angle), std::sin(angle)) * side_lengths[i]);
    }
    hexagon.setPointCount(6);
    for (size_t i = 0; i < 6; i++) hexagon.setPoint(i, points[i]);
    hexagon.setFillColor(sf::Color::Transparent);
    hexagon.setOutlineColor(sf::Color::White);
    hexagon.setOutlineThickness(2.f);

    sf::Vector2f center(0, 0);
    for (const auto& p : points) center += p;
    center /= float(points.size());
    boardCenter = center;

    // Hexagone extérieur (bordure noire épaisse)
    std::vector<sf::Vector2f> points2;
    std::vector<float> side_lengths2 = {500, 510, 510, 500, 510, 510};
    points2.push_back(origin + sf::Vector2f(side_lengths2[0], 0));
    for (int i = 1; i < 6; i++) {
        angle += PI_F / 3;
        points2.push_back(points2.back() +
            sf::Vector2f(std::cos(angle), std::sin(angle)) * side_lengths2[i]);
    }
    sf::Vector2f center2(0, 0);
    for (const auto& p : points2) center2 += p;
    center2 /= float(points2.size());
    sf::Vector2f offset = center - center2;
    for (auto& p : points2) p += offset;

    hexagon2.setPointCount(6);
    for (size_t i = 0; i < 6; i++) hexagon2.setPoint(i, points2[i]);
    hexagon2.setFillColor(sf::Color::White);
    hexagon2.setOutlineColor(sf::Color::Black);
    hexagon2.setOutlineThickness(7.f);

    // Milieux des arêtes
    std::vector<sf::Vector2f> milieux = {
        milieu(points[4], points[3]),
        milieu(points[3], points[2]),
        milieu(points[0], points[1]),
        milieu(points[0], points[5]),
        milieu(points[5], points[4]),
        milieu(points[1], points[2])
    };

    // Lignes de séparation des 6 matrices (depuis le centre vers chaque milieu)
    lines.clear();
    for (const auto& m : milieux)
        lines.push_back(createLine(center, m, sf::Color::Red));

    // Subdivision en losanges pour chaque matrice
    auto mat1_lines = createMatrixLines(center, milieux[1], milieux[0], points[3], points[4], points[2]);
    auto mat2_lines = createMatrixLines(center, milieux[5], milieux[1], points[2], points[3], points[1]);
    auto mat3_lines = createMatrixLines(center, milieux[0], milieux[4], points[4], points[5], points[3]);
    auto mat4_lines = createMatrixLines(center, milieux[4], milieux[3], points[5], points[0], points[4]);
    auto mat5_lines = createMatrixLines(center, milieux[2], milieux[5], points[1], points[2], points[0]);
    auto mat6_lines = createMatrixLines(center, milieux[3], milieux[2], points[0], points[1], points[5]);

    sf::Color blanc(0xFE, 0xF7, 0xE5);
    sf::Color beige(0xEE, 0xCF, 0xA1);
    matrice1 = createMatrixLosange(center, mat1_lines, points[3], milieux[1], milieux[0], beige, blanc);
    matrice2 = createMatrixLosange(center, mat2_lines, points[2], milieux[5], milieux[1], blanc, beige);
    matrice3 = createMatrixLosange(center, mat3_lines, points[4], milieux[0], milieux[4], blanc, beige);
    matrice4 = createMatrixLosange(center, mat4_lines, points[5], milieux[4], milieux[3], beige, blanc);
    matrice5 = createMatrixLosange(center, mat5_lines, points[1], milieux[2], milieux[5], beige, blanc);
    matrice6 = createMatrixLosange(center, mat6_lines, points[0], milieux[3], milieux[2], blanc, beige);

    // Labels autour du plateau (a-h, i-l, 1-12) identiques à projetexp
    std::vector<std::string> lettresBas         = {"a","b","c","d","e","f","g","h"};
    std::vector<std::string> lettresHaut        = {"8","7","6","5","9","10","11","12"};
    std::vector<std::string> chiffresBasGauche  = {"1","2","3","4","5","6","7","8"};
    std::vector<std::string> lettresHautDroite  = {"h","g","f","e","i","j","k","l"};
    std::vector<std::string> lettresHautGauche  = {"l","k","j","i","d","c","b","a"};
    std::vector<std::string> lettresBasDroite   = {"1","2","3","4","9","10","11","12"};

    sf::Vector2f startBas        = milieu(points[3], points2[4]) - sf::Vector2f(-30.f, 18.f);
    sf::Vector2f startHaut       = milieu(points[5], points2[0]) - sf::Vector2f(-30.f, 18.f);
    sf::Vector2f startBasGauche  = milieu(points[4], points2[5]) + sf::Vector2f(213.f, 370.f);
    sf::Vector2f startHautDroite = milieu(points[0], points2[1]) + sf::Vector2f(213.f, 360.f);
    sf::Vector2f startHautGauche = milieu(points[5], points2[0]) - sf::Vector2f(35.f, -15.f);
    sf::Vector2f startBasDroite  = milieu(points[2], points2[3]) + sf::Vector2f(7.f, -55.f);

    coordText.clear();
    for (size_t i = 0; i < 8; ++i) {
        coordText.push_back(createText(lettresBas[i],
            startBas + sf::Vector2f(i * 57.0f, 0.f), 28, sf::Color::Black));
        if (i > 4) {
            coordText.push_back(createText(lettresHaut[i],
                startHaut + sf::Vector2f(i * 56.0f, 0.f), 28, sf::Color::Black));
        } else {
            coordText.push_back(createText(lettresHaut[i],
                startHaut + sf::Vector2f(i * 57.0f, 0.f), 28, sf::Color::Black));
        }
        coordText.push_back(createText(chiffresBasGauche[i],
            startBasGauche - sf::Vector2f(i * 29.0f, i * 50.0f), 28, sf::Color::Black));
        coordText.push_back(createText(lettresHautDroite[i],
            startHautDroite - sf::Vector2f(i * 29.0f, i * 50.0f), 28, sf::Color::Black));
        coordText.push_back(createText(lettresHautGauche[i],
            startHautGauche + sf::Vector2f(7.f - i * 29.0f, i * 50.0f), 28, sf::Color::Black));
        if (i > 4) {
            coordText.push_back(createText(lettresBasDroite[i],
                startBasDroite + sf::Vector2f(i * 28.0f, 7.f - i * 50.0f), 28, sf::Color::Black));
        } else {
            coordText.push_back(createText(lettresBasDroite[i],
                startBasDroite + sf::Vector2f(i * 29.0f, 7.f - i * 50.0f), 28, sf::Color::Black));
        }
    }
}

// ── Mapping Position cubique ↔ (matrice, losange) ─────────────
// BLANC (mat1+mat2) : s=4-depth, r=-col, q=col+depth-4
// ROUGE (mat4+mat3) : q=4-depth, s=-col, r=col+depth-4
// NOIR  (mat5+mat6) : r=4-depth, q=-col, s=col+depth-4
void VueSFML::buildMapping() {
    posToTile.clear();
    tileToPos.clear();

    auto add = [&](int q, int r, int s, int mat, int los) {
        Position p(q, r, s);
        posToTile[p] = {mat, los};
        tileToPos[{mat, los}] = p;
    };

    // BLANC — mat1 (col 0..3) + mat2 (col 4..7)
    for (int d = 0; d <= 3; ++d) {
        for (int c = 0; c <= 7; ++c) {
            int s = 4 - d, r = -c, q = c + d - 4;
            if (c < 4)
                add(q, r, s, 1, 15 - 4*c - d);
            else
                add(q, r, s, 2, 4*(3 - d) + (c - 4));
        }
    }
    // ROUGE — mat4 (col 0..3) + mat3 (col 4..7)
    for (int d = 0; d <= 3; ++d) {
        for (int c = 0; c <= 7; ++c) {
            int q = 4 - d, s = -c, r = c + d - 4;
            if (c < 4)
                add(q, r, s, 4, 15 - 4*c - d);
            else
                add(q, r, s, 3, 4*(3 - d) + (c - 4));
        }
    }
    // NOIR — mat5 (col 0..3) + mat6 (col 4..7)
    for (int d = 0; d <= 3; ++d) {
        for (int c = 0; c <= 7; ++c) {
            int r = 4 - d, q = -c, s = c + d - 4;
            if (c < 4)
                add(q, r, s, 5, 15 - 4*c - d);
            else
                add(q, r, s, 6, 4*(3 - d) + (c - 4));
        }
    }
}

// ── Rendu ────────────────────────────────────────────────────
void VueSFML::clear() {
    window.clear(sf::Color(48, 46, 43));
}

void VueSFML::drawBoard() {
    window.draw(hexagon2);
    for (const auto& l : matrice1) window.draw(l);
    for (const auto& l : matrice2) window.draw(l);
    for (const auto& l : matrice3) window.draw(l);
    for (const auto& l : matrice4) window.draw(l);
    for (const auto& l : matrice5) window.draw(l);
    for (const auto& l : matrice6) window.draw(l);
    for (const auto& line : lines)
        window.draw(line.data(), line.size(), sf::PrimitiveType::Lines);
    window.draw(hexagon);
}

void VueSFML::drawCoords() {
    for (const auto& t : coordText) window.draw(t);
}

void VueSFML::drawPieces() {
    for (const auto& [pos, tile] : posToTile) {
        const Case* ca = jeu.getPlateau().obtenirCase(pos);
        if (!ca || !ca->estOccupee()) continue;
        const Piece* p = ca->getPiece();
        const sf::ConvexShape* los = getLosange(tile.first, tile.second);
        if (!los) continue;

        sf::Vector2f centre = calculerCentreLosange(*los);
        std::string cle = getCleTexture(p);
        bool hasTex = !cle.empty() && textures.count(cle) &&
                      textures.at(cle).getSize().x > 0;

        if (hasTex) {
            sf::Sprite sprite(textures.at(cle));
            sf::Vector2u ts = textures.at(cle).getSize();
            float scale = 50.f / float(std::max(ts.x, ts.y));
            sprite.setScale({scale, scale});
            auto bnd = sprite.getGlobalBounds();
            sprite.setPosition({centre.x - bnd.size.x * .5f,
                                centre.y - bnd.size.y * .5f});
            window.draw(sprite);
        } else {
            float rad = 18.f;
            sf::CircleShape circ(rad);
            circ.setOrigin({rad, rad});
            circ.setPosition(centre);
            if      (p->getCouleur() == Couleur::BLANC) circ.setFillColor(sf::Color(240,235,218));
            else if (p->getCouleur() == Couleur::ROUGE) circ.setFillColor(sf::Color(210,55,55));
            else                                         circ.setFillColor(sf::Color(22,22,22));
            circ.setOutlineColor(sf::Color(0,0,0,160));
            circ.setOutlineThickness(1.5f);
            window.draw(circ);
            sf::Text t(font, p->getSymbole(), 14u);
            t.setFillColor(p->getCouleur() == Couleur::BLANC
                           ? sf::Color(10,10,10) : sf::Color(240,240,240));
            auto b = t.getGlobalBounds();
            t.setPosition({centre.x - b.size.x*.5f - b.position.x,
                           centre.y - b.size.y*.5f - b.position.y});
            window.draw(t);
        }
    }
}

void VueSFML::drawHighlights() {
    if (!controleur.aPieceSelectionnee()) return;
    Position sel = controleur.getPositionSelectionnee();

    auto it = posToTile.find(sel);
    if (it != posToTile.end()) {
        const sf::ConvexShape* los = getLosange(it->second.first, it->second.second);
        if (los) {
            sf::ConvexShape hl = *los;
            hl.setFillColor(sf::Color(255, 220, 40, 210));
            window.draw(hl);
        }
    }

    const Case* ca = jeu.getPlateau().obtenirCase(sel);
    if (ca && ca->estOccupee()) {
        for (const auto& mv : ca->getPiece()->mouvementsPossibles(jeu.getPlateau())) {
            auto jt = posToTile.find(mv);
            if (jt == posToTile.end()) continue;
            const sf::ConvexShape* los = getLosange(jt->second.first, jt->second.second);
            if (!los) continue;
            sf::ConvexShape d = *los;
            d.setFillColor(sf::Color(50, 200, 50, 170));
            window.draw(d);
        }
    }
}

void VueSFML::drawInfo() {
    float py = float(WIN_H - INFO_H);
    sf::RectangleShape panel({float(WIN_W), float(INFO_H)});
    panel.setPosition({0.f, py});
    panel.setFillColor(sf::Color(16, 14, 11));
    window.draw(panel);

    sf::RectangleShape sep({float(WIN_W), 2.f});
    sep.setPosition({0.f, py});
    sep.setFillColor(sf::Color(75, 60, 40));
    window.draw(sep);

    Joueur* j = jeu.getJoueurCourant();
    if (j) {
        sf::Color ct;
        std::string cn;
        if      (j->getCouleur() == Couleur::BLANC) { cn = "Blanc"; ct = sf::Color(240,235,218); }
        else if (j->getCouleur() == Couleur::ROUGE) { cn = "Rouge"; ct = sf::Color(220,80,80);   }
        else                                         { cn = "Noir";  ct = sf::Color(130,200,130); }
        sf::Text t(font, "Tour : " + cn + "  (" + j->getNom() + ")", 18u);
        t.setFillColor(ct);
        t.setPosition({16.f, py + 8.f});
        window.draw(t);
    }
    if (!alerteMessage.empty()) {
        sf::Text alerte(font, alerteMessage, 18u);
        alerte.setFillColor(sf::Color(230, 60, 60));
        alerte.setPosition({400.f, py + 8.f});
        window.draw(alerte);
    } else {
        sf::Text hint(font, "Clic gauche : selectionner / deplacer", 12u);
        hint.setFillColor(sf::Color(95, 85, 65));
        hint.setPosition({16.f, py + 38.f});
        window.draw(hint);
    }
}

void VueSFML::drawBackButton() {
    window.draw(backButton);
    window.draw(backButtonText);
}

// ── Hit-test souris ──────────────────────────────────────────
Position VueSFML::sourisVersPosition(int mx, int my) const {
    sf::Vector2f pt{float(mx), float(my)};
    const std::vector<const std::vector<sf::ConvexShape>*> all = {
        &matrice1, &matrice2, &matrice3, &matrice4, &matrice5, &matrice6
    };
    for (int m = 0; m < 6; ++m) {
        const auto& mat = *all[m];
        for (int i = 0; i < (int)mat.size(); ++i) {
            if (pointDansLosange(mat[i], pt)) {
                auto it = tileToPos.find({m + 1, i});
                if (it != tileToPos.end()) return it->second;
            }
        }
    }
    return INVALID_POS();
}

// ── Accès matrices / losanges ────────────────────────────────
std::vector<sf::ConvexShape>& VueSFML::getMatrice(int mat) {
    switch (mat) {
        case 1: return matrice1;
        case 2: return matrice2;
        case 3: return matrice3;
        case 4: return matrice4;
        case 5: return matrice5;
        case 6: return matrice6;
        default: throw std::out_of_range("Index de matrice invalide");
    }
}

const sf::ConvexShape* VueSFML::getLosange(int mat, int los) const {
    const std::vector<const std::vector<sf::ConvexShape>*> all = {
        &matrice1, &matrice2, &matrice3, &matrice4, &matrice5, &matrice6
    };
    if (mat < 1 || mat > 6) return nullptr;
    const auto& m = *all[mat - 1];
    if (los < 0 || los >= (int)m.size()) return nullptr;
    return &m[los];
}

sf::Vector2f VueSFML::calculerCentreLosange(const sf::ConvexShape& s) const {
    return {(s.getPoint(0).x + s.getPoint(2).x) * 0.5f,
            (s.getPoint(0).y + s.getPoint(2).y) * 0.5f};
}

bool VueSFML::pointDansLosange(const sf::ConvexShape& s, sf::Vector2f pt) const {
    int n = (int)s.getPointCount();
    bool inside = false;
    for (int i = 0, j = n - 1; i < n; j = i++) {
        sf::Vector2f pi = s.getPoint(i);
        sf::Vector2f pj = s.getPoint(j);
        if (((pi.y > pt.y) != (pj.y > pt.y)) &&
            (pt.x < (pj.x - pi.x) * (pt.y - pi.y) / (pj.y - pi.y) + pi.x))
            inside = !inside;
    }
    return inside;
}

// ── Helpers de construction (adaptés de MakeBoard) ───────────
sf::Vector2f VueSFML::milieu(const sf::Vector2f& a, const sf::Vector2f& b) const {
    return (a + b) / 2.f;
}

std::array<sf::Vertex, 2> VueSFML::createLine(const sf::Vector2f& a,
                                              const sf::Vector2f& b,
                                              sf::Color c) const {
    std::array<sf::Vertex, 2> arr;
    arr[0].position = a; arr[0].color = c;
    arr[1].position = b; arr[1].color = c;
    return arr;
}

std::vector<sf::Vector2f> VueSFML::createMatrixLines(
    const sf::Vector2f& center,
    const sf::Vector2f& m1, const sf::Vector2f& m2,
    const sf::Vector2f& p1, const sf::Vector2f& p2,
    const sf::Vector2f& p3) const
{
    std::vector<sf::Vector2f> res;
    for (float ratio : {0.25f, 0.50f, 0.75f}) {
        res.push_back(center + ratio * (m1 - center));
        res.push_back((1 - (1 - ratio) / 2) * p1 + ((1 - ratio) / 2) * p2);
    }
    for (float ratio : {0.25f, 0.50f, 0.75f}) {
        res.push_back(center + ratio * (m2 - center));
        res.push_back((1 - (1 + ratio) / 2) * p3 + ((1 + ratio) / 2) * p1);
    }
    return res;
}

sf::ConvexShape VueSFML::createLosange(
    const sf::Vector2f& p1, const sf::Vector2f& p2,
    const sf::Vector2f& p3, const sf::Vector2f& p4,
    sf::Color fill, sf::Color outline) const
{
    sf::ConvexShape l;
    l.setPointCount(4);
    l.setPoint(0, p1);
    l.setPoint(1, p2);
    l.setPoint(2, p3);
    l.setPoint(3, p4);
    l.setFillColor(fill);
    l.setOutlineColor(outline);
    l.setOutlineThickness(2.f);
    return l;
}

std::vector<sf::ConvexShape> VueSFML::createMatrixLosange(
    const sf::Vector2f& center,
    const std::vector<sf::Vector2f>& L,
    const sf::Vector2f& point,
    const sf::Vector2f& m1, const sf::Vector2f& m2,
    sf::Color c1, sf::Color c2) const
{
    std::vector<sf::ConvexShape> shapes;
    // Ligne 0 (près du centre)
    shapes.push_back(createLosange(center, L[0], L[6] + 0.25f*(L[7]-L[6]), L[6], c1, sf::Color::Black));
    shapes.push_back(createLosange(L[0], L[2], L[6] + 0.5f*(L[7]-L[6]), L[6] + 0.25f*(L[7]-L[6]), c2, sf::Color::Black));
    shapes.push_back(createLosange(L[2], L[4], L[6] + 0.75f*(L[7]-L[6]), L[6] + 0.5f*(L[7]-L[6]), c1, sf::Color::Black));
    shapes.push_back(createLosange(L[4], m1, L[7], L[6] + 0.75f*(L[7]-L[6]), c2, sf::Color::Black));
    // Ligne 1
    shapes.push_back(createLosange(L[6], L[6] + 0.25f*(L[7]-L[6]), L[8] + 0.25f*(L[9]-L[8]), L[8], c2, sf::Color::Black));
    shapes.push_back(createLosange(L[6] + 0.25f*(L[7]-L[6]), L[6] + 0.5f*(L[7]-L[6]), L[8] + 0.5f*(L[9]-L[8]), L[8] + 0.25f*(L[9]-L[8]), c1, sf::Color::Black));
    shapes.push_back(createLosange(L[6] + 0.5f*(L[7]-L[6]), L[6] + 0.75f*(L[7]-L[6]), L[8] + 0.75f*(L[9]-L[8]), L[8] + 0.5f*(L[9]-L[8]), c2, sf::Color::Black));
    shapes.push_back(createLosange(L[6] + 0.75f*(L[7]-L[6]), L[7], L[9], L[8] + 0.75f*(L[9]-L[8]), c1, sf::Color::Black));
    // Ligne 2
    shapes.push_back(createLosange(L[8], L[8] + 0.25f*(L[9]-L[8]), L[10] + 0.25f*(L[11]-L[10]), L[10], c1, sf::Color::Black));
    shapes.push_back(createLosange(L[8] + 0.25f*(L[9]-L[8]), L[8] + 0.5f*(L[9]-L[8]), L[10] + 0.5f*(L[11]-L[10]), L[10] + 0.25f*(L[11]-L[10]), c2, sf::Color::Black));
    shapes.push_back(createLosange(L[8] + 0.5f*(L[9]-L[8]), L[8] + 0.75f*(L[9]-L[8]), L[10] + 0.75f*(L[11]-L[10]), L[10] + 0.5f*(L[11]-L[10]), c1, sf::Color::Black));
    shapes.push_back(createLosange(L[8] + 0.75f*(L[9]-L[8]), L[9], L[11], L[10] + 0.75f*(L[11]-L[10]), c2, sf::Color::Black));
    // Ligne 3 (vers l'apex)
    shapes.push_back(createLosange(L[10], L[10] + 0.25f*(L[11]-L[10]), L[1], m2, c2, sf::Color::Black));
    shapes.push_back(createLosange(L[10] + 0.25f*(L[11]-L[10]), L[10] + 0.5f*(L[11]-L[10]), L[3], L[1], c1, sf::Color::Black));
    shapes.push_back(createLosange(L[10] + 0.5f*(L[11]-L[10]), L[10] + 0.75f*(L[11]-L[10]), L[5], L[3], c2, sf::Color::Black));
    shapes.push_back(createLosange(L[10] + 0.75f*(L[11]-L[10]), L[5], point, L[11], c1, sf::Color::Black));
    return shapes;
}

sf::Text VueSFML::createText(const std::string& s, sf::Vector2f pos,
                              unsigned size, sf::Color col)
{
    sf::Text t(font, s, size);
    t.setFillColor(col);
    t.setPosition(pos);
    return t;
}

// ── Patron Observateur ───────────────────────────────────────
// Appelé par Jeu::notifierObservateurs() après chaque coup.
// Construit le message d'alerte selon l'état courant de la partie.
void VueSFML::mettreAJour() {
    alerteMessage = "";
    EtatPartie etat = jeu.getEtatPartie();

    if (etat == EtatPartie::TERMINEE) {
        // Trouver le joueur non éliminé = gagnant
        for (Joueur* j : jeu.getJoueurs()) {
            if (!j->getEstElimine()) {
                alerteMessage = "Partie terminee — Gagnant : " + j->getNom();
                break;
            }
        }
        return;
    }

    if (etat == EtatPartie::MAT) {
        // Identifier le joueur qui vient d'être éliminé
        for (Joueur* j : jeu.getJoueurs()) {
            if (j->getEstElimine()) {
                alerteMessage = "MAT ! " + j->getNom() + " est elimine.";
                break;
            }
        }
        return;
    }

    if (etat == EtatPartie::ECHEC) {
        // Identifier quel(s) roi(s) sont menacés
        for (Joueur* j : jeu.getJoueurs()) {
            if (!j->getEstElimine() && jeu.estEnEchec(j)) {
                if (!alerteMessage.empty()) alerteMessage += "  |  ";
                alerteMessage += "ECHEC ! " + j->getNom();
            }
        }
    }
}

// ── Boucle principale ────────────────────────────────────────
void VueSFML::run() {
    while (window.isOpen()) {
        while (const auto ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>())
                window.close();
            if (const auto* mb = ev->getIf<sf::Event::MouseButtonPressed>()) {
                if (mb->button == sf::Mouse::Button::Left) {
                    Position pos = sourisVersPosition(mb->position.x, mb->position.y);
                    std::cerr << "[CLICK] pixel=(" << mb->position.x << "," << mb->position.y
                              << ")  pos=(" << pos.q << "," << pos.r << "," << pos.s
                              << ")  valide=" << pos.estValide() << "\n";
                    if (pos.estValide()) {
                        const Case* c = jeu.getPlateau().obtenirCase(pos);
                        std::cerr << "  case existe=" << (c != nullptr)
                                  << "  occupee=" << (c && c->estOccupee())
                                  << "\n";
                        if (c && c->estOccupee()) {
                            Piece* p = c->getPiece();
                            std::cerr << "  piece=" << p->getSymbole()
                                      << "  joueur=" << p->getJoueur()->getNom()
                                      << "  tour=" << jeu.getJoueurCourant()->getNom()
                                      << "\n";
                        }
                        controleur.gererClicCase(pos);
                        std::cerr << "  apres: pieceSelectionnee=" << controleur.aPieceSelectionnee() << "\n";
                    }
                }
            }
        }

        clear();
        drawBoard();
        drawHighlights();
        drawCoords();
        drawPieces();
        drawInfo();
        drawBackButton();
        window.display();
    }
}
