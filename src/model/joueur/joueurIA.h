#pragma once

#include "Joueur.h"
#include "../base/Position.h"
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <climits>
#include <string>
#include <unordered_map>

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
    std::vector<Coup> genererCoups(Joueur* j, Plateau& p) const;
    void appliquerCoup(Coup& c, Plateau& p);
    void annulerCoup(const Coup& c, Plateau& p);

    // ── Hachage de position ───────────────────────────────────────────────
    /** Encode l'état du plateau en chaîne déterministe (pour détecter les répétitions) */
    std::string hashPlateau(const Plateau& p) const;

    /** Positions réelles jouées : hash → nombre d'apparitions */
    std::unordered_map<std::string, int> historiquePartie;

    // ── Évaluation ───────────────────────────────────────────────────────
    int evaluerPosition(Plateau& p) const;

    // ── Recherche alpha-beta ──────────────────────────────────────────────
    /**
     * @param chemin  hachages des positions sur le chemin courant (détection répétition)
     */
    int alphaBeta(Plateau& p, int profondeur, int alpha, int beta,
                  int indexCourant, const std::vector<Joueur*>& joueurs,
                  std::vector<std::string>& chemin);

    void threadJouer();

public:
    JoueurIA(const std::string& nom, Couleur couleur, int profondeur);
    ~JoueurIA();

    void setJeu(Jeu* j) { jeu = j; }

    /** Enregistre la position courante du plateau dans l'historique de la partie */
    void enregistrerPosition(const Plateau& p);

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
