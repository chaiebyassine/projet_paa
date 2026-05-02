#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <utility>

#include "../model/jeu.h"
#include "../model/observateur.h"
#include "../model/joueur/JoueurIA.h"
#include "../model/joueur/Joueur.h"
#include "../model/base/position.h"
#include "../controller/controleurjeu.h"
#include "../model/piece/piece.h"

// Vue SFML — plateau Yalta 3 joueurs.
// Affichage : grand hexagone divisé en 6 matrices triangulaires,
// chaque matrice subdivisée en 16 losanges (= 96 cases au total).
// Implémente Observateur : s'enregistre auprès du Jeu à la construction
// et met à jour le panneau d'alerte à chaque notification.
// Contient son propre Jeu (valeur, pas référence) et gère les joueurs.
class VueSFML : public Observateur {
public:
    enum class Ecran { MENU, JEU, PAUSE };

    static constexpr unsigned WIN_W  = 1200u;
    static constexpr unsigned WIN_H  = 1070u;
    static constexpr unsigned INFO_H = 70u;

    VueSFML();
    void run();

    // Patron Observateur : appelé automatiquement par Jeu après chaque coup
    void mettreAJour() override;

private:
    Jeu           jeu;        // valeur — adresse stable pour le contrôleur
    ControleurJeu controleur;
    sf::RenderWindow window;
    sf::Font         font;

    Ecran ecranActuel;
    int   nbJoueurIA;
    std::vector<std::unique_ptr<Joueur>> joueursGeres;

    // 6 matrices de 16 losanges = 96 cases
    std::vector<sf::ConvexShape> matrice1, matrice2, matrice3,
                                 matrice4, matrice5, matrice6;

    // Hexagones bordure et lignes de séparation des matrices
    sf::ConvexShape hexagon;
    sf::ConvexShape hexagon2;
    std::vector<std::array<sf::Vertex, 2>> lines;

    // Labels de coordonnées autour du plateau
    std::vector<sf::Text> coordText;

    // Textures des pièces (clé = ex "WhitePawn")
    std::map<std::string, sf::Texture> textures;

    // Mapping bidirectionnel Position cubique ↔ (indexMatrice, indexLosange)
    std::map<Position, std::pair<int,int>> posToTile;
    std::map<std::pair<int,int>, Position> tileToPos;

    // Panneau info & bouton retour
    sf::Text           textGame;
    sf::Text           textEchec;
    sf::RectangleShape backButton;
    sf::Text           backButtonText;

    // Message d'alerte mis à jour par mettreAJour() (ex: "ECHEC ! — Alice")
    std::string alerteMessage;

    sf::Vector2f boardCenter;

    // ── Construction ──────────────────────────────────────────
    void buildBoard();
    void buildMapping();
    void chargerTextures();
    void demarrerPartie(int nbIA);

    // ── Rendu ─────────────────────────────────────────────────
    void clear();
    void drawBoard();
    void drawPieces();
    void drawHighlights();
    void drawCoords();
    void drawInfo();
    void drawBackButton();
    void drawMenu();
    void drawPause();

    // ── Interaction ───────────────────────────────────────────
    Position sourisVersPosition(int mx, int my) const;
    std::string getCleTexture(const Piece* p) const;
    void handleMenuClick(int x, int y);
    void handleGameClick(int x, int y);
    void handlePauseClick(int x, int y);

    // ── Helpers géométriques (adaptés de MakeBoard de projetexp) ─
    std::vector<sf::ConvexShape>& getMatrice(int mat);
    const sf::ConvexShape* getLosange(int mat, int los) const;
    sf::Vector2f calculerCentreLosange(const sf::ConvexShape& s) const;
    bool pointDansLosange(const sf::ConvexShape& s, sf::Vector2f p) const;

    sf::Vector2f milieu(const sf::Vector2f& a, const sf::Vector2f& b) const;
    std::array<sf::Vertex, 2> createLine(const sf::Vector2f& a,
                                         const sf::Vector2f& b,
                                         sf::Color c) const;
    std::vector<sf::Vector2f> createMatrixLines(
        const sf::Vector2f& center,
        const sf::Vector2f& m1, const sf::Vector2f& m2,
        const sf::Vector2f& p1, const sf::Vector2f& p2,
        const sf::Vector2f& p3) const;
    sf::ConvexShape createLosange(
        const sf::Vector2f& p1, const sf::Vector2f& p2,
        const sf::Vector2f& p3, const sf::Vector2f& p4,
        sf::Color fill, sf::Color outline) const;
    std::vector<sf::ConvexShape> createMatrixLosange(
        const sf::Vector2f& center,
        const std::vector<sf::Vector2f>& mat_lines,
        const sf::Vector2f& point,
        const sf::Vector2f& m1, const sf::Vector2f& m2,
        sf::Color c1, sf::Color c2) const;
    sf::Text createText(const std::string& s, sf::Vector2f pos,
                        unsigned size, sf::Color col);
};
