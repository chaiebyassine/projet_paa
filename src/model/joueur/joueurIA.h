#pragma once

#include "Joueur.h"
#include "../base/Position.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <climits>

class Jeu;
class Plateau;

/**
 * @brief Joueur contrôlé par l'IA.
 *
 * Algorithme : alpha-beta élagage avec approche paranoïaque pour 3 joueurs.
 * L'IA maximise son propre score ; tous les autres joueurs sont traités comme
 * des adversaires qui le minimisent (paranoid minimax).
 *
 * Multi-threading : le calcul tourne dans un thread dédié (gestionnaireThreads)
 * pour ne pas bloquer l'interface graphique. Au niveau de la racine, les coups
 * sont répartis entre plusieurs threads workers qui s'alternent sur le plateau
 * via un mutex.
 */
class JoueurIA : public Joueur {
public:
    /** Représentation allégée d'un coup pour la simulation interne */
    struct Coup {
        Piece*   piece;
        Position depart;
        Position arrivee;
        Piece*   pieceCapturee;   ///< nullptr si pas de capture
        Joueur*  joueurCapture;   ///< propriétaire de la pièce capturée
        bool     avaitDejaBouge;  ///< flag avant le coup (restauré lors de l'annulation)
    };

private:
    int               profondeurRecherche; ///< profondeur de l'arbre de recherche
    Jeu*              jeu;                 ///< référence au modèle
    std::thread       gestionnaireThreads; ///< thread principal de calcul IA
    std::atomic<bool> calcEnCours;         ///< vrai pendant toute la durée du calcul
    std::atomic<bool> arret;               ///< signal d'arrêt anticipé (ex: fermeture)

    // ── Outils de simulation ──────────────────────────────────────────────
    /** Génère tous les coups pseudo-légaux pour un joueur */
    std::vector<Coup> genererCoups(Joueur* j, Plateau& p) const;

    /** Applique un coup sur le plateau (make) */
    void appliquerCoup(Coup& c, Plateau& p);

    /** Annule un coup précédemment appliqué (unmake) */
    void annulerCoup(const Coup& c, Plateau& p);

    // ── Évaluation ───────────────────────────────────────────────────────
    /** Heuristique : matériel + mobilité, du point de vue de cette IA */
    int evaluerPosition(Plateau& p) const;

    // ── Recherche alpha-beta ──────────────────────────────────────────────
    /**
     * @brief Alpha-beta paranoïaque pour N joueurs.
     * @param indexCourant index dans joueurs[] du joueur qui doit jouer
     */
    int alphaBeta(Plateau& p, int profondeur, int alpha, int beta,
                  int indexCourant, const std::vector<Joueur*>& joueurs);

    /** Corps du thread : calcule puis applique le meilleur coup */
    void threadJouer();

public:
    JoueurIA(const std::string& nom, Couleur couleur, int profondeur);
    ~JoueurIA();

    /** Fournit la référence au jeu (appelé par Jeu::ajouterJoueur) */
    void setJeu(Jeu* j) { jeu = j; }

    /** Vrai si le calcul est en cours */
    bool estCalculEnCours() const { return calcEnCours.load(); }

    /** Demande l'arrêt anticipé (thread s'arrête au prochain nœud) */
    void signalerArret() { arret = true; }

    /** Lance le calcul en arrière-plan ; retourne immédiatement */
    void jouer() override;

    /** Valeur matérielle d'une pièce */
    static int valeurPiece(const Piece* p);

    /** Évalue la position courante (accès externe pour debug/rapport) */
    int evaluatePosition();
};
