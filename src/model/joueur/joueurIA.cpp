/**
 * @file joueurIA.cpp
 * @brief Implémentation du joueur IA : alpha-beta paranoïaque, multi-threadé.
 *
 * Approche paranoïaque (paranoid minimax) :
 *   - Le joueur IA est MAX : il cherche à maximiser son score.
 *   - Tous les autres joueurs sont MIN : ils cherchent à minimiser ce score.
 * Cela permet d'appliquer l'élagage alpha-beta standard même à 3 joueurs.
 *
 * Multi-threading :
 *   - jouer() lance un thread dédié (gestionnaireThreads) pour ne pas bloquer la vue.
 *   - Au niveau racine, un pool de threads workers se répartit les coups.
 *     Chaque thread verrouille un mutex avant d'appliquer/évaluer/annuler son coup,
 *     garantissant l'intégrité du plateau partagé.
 */

#include "JoueurIA.h"
#include "../jeu.h"
#include "../plateau/Plateau.h"
#include "../plateau/Case.h"
#include "../piece/Piece.h"
#include "../piece/Roi.h"
#include <iostream>
#include <algorithm>

// ── Constructeur / Destructeur ────────────────────────────────────────────────

JoueurIA::JoueurIA(const std::string& nom, Couleur couleur, int profondeur)
    : Joueur(nom, couleur), profondeurRecherche(profondeur),
      jeu(nullptr), calcEnCours(false), arret(false) {
}

JoueurIA::~JoueurIA() {
    arret = true;   // demande l'arrêt anticipé de la recherche
    if (gestionnaireThreads.joinable())
        gestionnaireThreads.join();
}

// ── Valeur matérielle des pièces ──────────────────────────────────────────────

int JoueurIA::valeurPiece(const Piece* p) {
    if (!p) return 0;
    const std::string& s = p->getSymbole();
    if (s == "R") return 10000;  // Roi   — perte = défaite
    if (s == "D") return 900;    // Reine
    if (s == "T") return 500;    // Tour
    if (s == "F") return 320;    // Fou
    if (s == "C") return 300;    // Cavalier
    if (s == "P") return 100;    // Pion
    return 0;
}

// ── Génération des coups ──────────────────────────────────────────────────────

std::vector<JoueurIA::Coup> JoueurIA::genererCoups(Joueur* j, Plateau& plateau) const {
    std::vector<Coup> coups;
    for (Piece* p : j->getPieces()) {
        for (const Position& dest : p->mouvementsPossibles(plateau)) {
            Case* cDest = plateau.obtenirCase(dest);
            Piece* cap  = (cDest && cDest->estOccupee()) ? cDest->getPiece() : nullptr;
            // Ignorer les cases occupées par une pièce alliée (sécurité)
            if (cap && cap->getCouleur() == j->getCouleur()) continue;
            Joueur* jCap = cap ? cap->getJoueur() : nullptr;
            coups.push_back({ p, p->getPosition(), dest, cap, jCap, p->getADejaBouge() });
        }
    }
    return coups;
}

// ── Make / Unmake ─────────────────────────────────────────────────────────────

void JoueurIA::appliquerCoup(Coup& c, Plateau& plateau) {
    // Retirer la pièce capturée de la liste du joueur adverse
    if (c.pieceCapturee && c.joueurCapture)
        c.joueurCapture->retirerPiece(c.pieceCapturee);

    plateau.deplacerPiece(c.depart, c.arrivee);
    c.piece->setPosition(c.arrivee);
    c.piece->setADejaBouge(true);
}

void JoueurIA::annulerCoup(const Coup& c, Plateau& plateau) {
    plateau.deplacerPiece(c.arrivee, c.depart);
    c.piece->setPosition(c.depart);
    c.piece->setADejaBouge(c.avaitDejaBouge);

    // Remettre la pièce capturée sur le plateau et dans la liste du joueur
    if (c.pieceCapturee && c.joueurCapture) {
        Case* cArr = plateau.obtenirCase(c.arrivee);
        if (cArr) cArr->placerPiece(c.pieceCapturee);
        c.joueurCapture->ajouterPiece(c.pieceCapturee);
    }
}

// ── Fonction d'évaluation ─────────────────────────────────────────────────────

/**
 * Évaluation paranoïaque : matériel propre - matériel ennemi + bonus de mobilité.
 * Retourne un score du point de vue de cette IA (plus c'est grand, mieux c'est).
 */
int JoueurIA::evaluerPosition(Plateau& plateau) const {
    int score = 0;

    for (const Position& pos : plateau.getToutesLesPositions()) {
        const Case* c = plateau.obtenirCase(pos);
        if (!c || !c->estOccupee()) continue;
        const Piece* p = c->getPiece();
        int val = valeurPiece(p);
        if (p->getCouleur() == this->couleur)
            score += val;
        else
            score -= val;
    }
    return score;
}

int JoueurIA::evaluatePosition() {
    if (!jeu) return 0;
    return evaluerPosition(jeu->getPlateau());
}

// ── Alpha-Beta paranoïaque ────────────────────────────────────────────────────

/**
 * Alpha-beta standard adapté pour N joueurs (approche paranoïaque) :
 *   - nœud MAX si c'est le tour de l'IA (couleur == this->couleur)
 *   - nœud MIN pour tous les autres joueurs
 *
 * @param indexCourant  index du joueur qui doit jouer à ce nœud
 */
int JoueurIA::alphaBeta(Plateau& plateau, int profondeur, int alpha, int beta,
                         int indexCourant, const std::vector<Joueur*>& joueurs) {
    // Arrêt anticipé (destruction de l'objet ou fermeture de la fenêtre)
    if (arret.load()) return evaluerPosition(plateau);

    if (profondeur == 0)
        return evaluerPosition(plateau);

    int nb = (int)joueurs.size();
    Joueur* joueurActuel = joueurs[indexCourant % nb];

    auto coups = genererCoups(joueurActuel, plateau);
    if (coups.empty()) return evaluerPosition(plateau);

    bool estMax = (joueurActuel->getCouleur() == this->couleur);
    int meilleur = estMax ? INT_MIN + 1 : INT_MAX - 1;

    for (auto& coup : coups) {
        if (arret.load()) break;

        appliquerCoup(coup, plateau);
        int score = alphaBeta(plateau, profondeur - 1, alpha, beta,
                              indexCourant + 1, joueurs);
        annulerCoup(coup, plateau);

        if (estMax) {
            meilleur = std::max(meilleur, score);
            alpha    = std::max(alpha,    meilleur);
        } else {
            meilleur = std::min(meilleur, score);
            beta     = std::min(beta,     meilleur);
        }
        if (beta <= alpha) break;   // coupure alpha-beta
    }
    return meilleur;
}

// ── Thread de calcul ──────────────────────────────────────────────────────────

void JoueurIA::threadJouer() {
    if (!jeu) { calcEnCours = false; return; }

    Plateau& plateau                    = jeu->getPlateau();
    const std::vector<Joueur*>& joueurs = jeu->getJoueurs();

    // Index de l'IA dans la liste des joueurs
    int monIndex = 0;
    for (int i = 0; i < (int)joueurs.size(); i++)
        if (joueurs[i] == this) { monIndex = i; break; }

    // Générer les coups disponibles à la racine
    auto coups = genererCoups(this, plateau);
    if (coups.empty()) { calcEnCours = false; return; }

    // ── Parallélisme niveau racine ────────────────────────────────────────
    // Chaque thread prend un coup à évaluer. L'accès au plateau est protégé
    // par boardMutex : les threads s'alternent (make → alphaBeta → unmake).
    // La meilleure évaluation est mise à jour sous resultMutex.
    const int NUM_THREADS = (int)std::max(1u, std::thread::hardware_concurrency());
    std::mutex boardMutex;
    std::mutex resultMutex;
    std::atomic<int> nextIdx{ 0 };

    int  meilleurScore   = INT_MIN + 1;
    int  meilleurCoupIdx = 0;

    auto worker = [&]() {
        while (!arret.load()) {
            int idx = nextIdx.fetch_add(1);
            if (idx >= (int)coups.size()) break;

            int score;
            {
                // Section critique : un seul thread à la fois sur le plateau
                std::lock_guard<std::mutex> lock(boardMutex);
                appliquerCoup(coups[idx], plateau);
                score = alphaBeta(plateau,
                                  profondeurRecherche - 1,
                                  INT_MIN + 1, INT_MAX - 1,
                                  (monIndex + 1) % (int)joueurs.size(),
                                  joueurs);
                annulerCoup(coups[idx], plateau);
            }

            {
                std::lock_guard<std::mutex> lock(resultMutex);
                if (score > meilleurScore) {
                    meilleurScore   = score;
                    meilleurCoupIdx = idx;
                }
            }
        }
    };

    // Lancer le pool de threads
    std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);
    for (int t = 0; t < NUM_THREADS; t++)
        threads.emplace_back(worker);
    for (auto& t : threads)
        t.join();

    // Appliquer le meilleur coup via le jeu (déclenche Observer + vérification état)
    if (!arret.load()) {
        const Coup& best = coups[meilleurCoupIdx];
        std::cout << "[IA] " << nom
                  << " : (" << best.depart.q  << "," << best.depart.r  << "," << best.depart.s  << ")"
                  << " → ("  << best.arrivee.q << "," << best.arrivee.r << "," << best.arrivee.s << ")"
                  << "  score=" << meilleurScore << "\n";
        jeu->deplacerPiece(best.depart, best.arrivee);
    }

    calcEnCours = false;
}

// ── Interface publique ────────────────────────────────────────────────────────

void JoueurIA::jouer() {
    if (calcEnCours.load()) return;   // déjà en train de calculer
    calcEnCours = true;

    if (gestionnaireThreads.joinable())
        gestionnaireThreads.join();   // attendre la fin du thread précédent

    gestionnaireThreads = std::thread(&JoueurIA::threadJouer, this);
}
