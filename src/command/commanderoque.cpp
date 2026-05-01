/**
 * @file commanderoque.cpp
 * @brief Implémentation du roque (mouvement spécial Roi + Tour).
 */
#include "commanderoque.h"
#include "../model/piece/Roi.h"
#include "../model/piece/Tour.h"
#include "../model/plateau/Plateau.h"
#include "../model/plateau/Case.h"
#include "../model/base/coord2d.h"
#include <algorithm>

CommandeRoque::CommandeRoque(Jeu* jeu, Roi* roi, Tour* tour,
                             const Position& posRoiArrivee,
                             const Position& posTourArrivee)
    : jeu(jeu), roi(roi), tour(tour),
      posRoiDepart(roi->getPosition()),
      posRoiArrivee(posRoiArrivee),
      posTourDepart(tour->getPosition()),
      posTourArrivee(posTourArrivee),
      executionReussie(false),
      roiAvaitBouge(roi->getADejaBouge()),
      tourAvaitBouge(tour->getADejaBouge()) {
}

void CommandeRoque::executer() {
    if (!jeu || !roi || !tour) return;
    if (!peutRoquer(*jeu, roi, tour)) return;

    Plateau& p = jeu->getPlateau();

    // Déplacer le Roi
    p.deplacerPiece(posRoiDepart, posRoiArrivee);
    roi->setPosition(posRoiArrivee);
    roi->setADejaBouge(true);

    // Déplacer la Tour
    p.deplacerPiece(posTourDepart, posTourArrivee);
    tour->setPosition(posTourArrivee);
    tour->setADejaBouge(true);

    executionReussie = true;
}

void CommandeRoque::annuler() {
    if (!executionReussie) return;

    Plateau& p = jeu->getPlateau();

    // Remettre la Tour à sa case initiale
    p.deplacerPiece(posTourArrivee, posTourDepart);
    tour->setPosition(posTourDepart);
    tour->setADejaBouge(tourAvaitBouge);

    // Remettre le Roi à sa case initiale
    p.deplacerPiece(posRoiArrivee, posRoiDepart);
    roi->setPosition(posRoiDepart);
    roi->setADejaBouge(roiAvaitBouge);

    executionReussie = false;
}

bool CommandeRoque::getExecutionReussie() const {
    return executionReussie;
}

// Vérifie que le chemin en 2D entre deux positions est entièrement vide.
// Fonctionne sur une même ligne (même x) ou une même colonne (même y).
// Les cases invalides (zone centrale de Yalta) sont ignorées.
static bool cheminLibre(const Plateau& plateau, const Position& de, const Position& a) {
    auto [x1, y1] = positionTo2D(de);
    auto [x2, y2] = positionTo2D(a);
    if (x1 < 0 || x2 < 0) return false;

    if (x1 == x2) {
        int yMin = std::min(y1, y2) + 1;
        int yMax = std::max(y1, y2) - 1;
        for (int y = yMin; y <= yMax; y++) {
            Position pos = positionFrom2D(x1, y);
            if (!pos.estValide() || !plateau.caseExiste(pos)) continue;
            const Case* c = plateau.obtenirCase(pos);
            if (c && c->estOccupee()) return false;
        }
        return true;
    }
    if (y1 == y2) {
        int xMin = std::min(x1, x2) + 1;
        int xMax = std::max(x1, x2) - 1;
        for (int x = xMin; x <= xMax; x++) {
            Position pos = positionFrom2D(x, y1);
            if (!pos.estValide() || !plateau.caseExiste(pos)) continue;
            const Case* c = plateau.obtenirCase(pos);
            if (c && c->estOccupee()) return false;
        }
        return true;
    }
    return false; // Roi et Tour ne partagent pas la même ligne
}

bool CommandeRoque::peutRoquer(Jeu& jeu, Roi* roi, Tour* tour) {
    if (!roi || !tour) return false;

    // Condition 1 : aucune des deux pièces ne doit avoir bougé
    if (roi->getADejaBouge() || tour->getADejaBouge()) return false;

    // Condition 2 : le roi ne doit pas être en échec
    if (jeu.estEnEchec(roi->getJoueur())) return false;

    // Condition 3 : chemin libre entre le roi et la tour
    if (!cheminLibre(jeu.getPlateau(), roi->getPosition(), tour->getPosition()))
        return false;

    return true;
}
