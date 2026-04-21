/**
 * @file jeu.cpp
 * @brief Implémentation de la classe Jeu.
 */
#include "Jeu.h"
#include "plateau/Case.h"
#include "piece/Piece.h"
#include "piece/Tour.h"
#include "piece/Roi.h"
#include "piece/Pion.h"
#include "piece/Cavalier.h"
#include "piece/Fou.h"
#include "piece/Reine.h"
#include "piece/Pion.h"
#include "piece/Cavalier.h"
#include "piece/Fou.h"
#include "piece/Reine.h"
#include "joueur/Joueur.h"


// Constructeur : crée un jeu avec un plateau de taille donnée
// Le joueur courant est le premier (index 0) et la partie est en cours
Jeu::Jeu(int lignes, int colonnes)
    : plateau(lignes, colonnes), indexJoueurCourant(0), etatPartie(EtatPartie::EN_COURS) {
}

// Ajoute un joueur au jeu (maximum 3 joueurs autorisés)
void Jeu::ajouterJoueur(Joueur* joueur) {
    if (joueurs.size() < 3) {
        joueurs.push_back(joueur);
    }
}

// Retourne une référence modifiable vers le plateau
Plateau& Jeu::getPlateau() {
    return plateau;
}

// Retourne une référence constante vers le plateau (lecture seule)
const Plateau& Jeu::getPlateau() const {
    return plateau;
}

// Retourne le joueur dont c'est le tour de jouer
Joueur* Jeu::getJoueurCourant() const {
    if (joueurs.empty()) {
        return nullptr;  // Aucun joueur enregistré
    }
    return joueurs[indexJoueurCourant];
}

// Retourne l'état actuel de la partie
EtatPartie Jeu::getEtatPartie() const {
    return etatPartie;
}

// Passe au joueur suivant en utilisant le modulo pour boucler entre les 3 joueurs
void Jeu::changerJoueur() {
    if (joueurs.empty()) {
        return;
    }

    // Le modulo permet de revenir au joueur 0 après le dernier joueur
    indexJoueurCourant = (indexJoueurCourant + 1) % joueurs.size();
}

// Déplace une pièce de la position de départ vers la position d'arrivée
// Vérifie toutes les règles : propriété de la pièce, validité du mouvement, capture
bool Jeu::deplacerPiece(const Position& depart, const Position& arrivee) {
    // Récupère les cases de départ et d'arrivée
    Case* caseDepart = plateau.obtenirCase(depart);
    Case* caseArrivee = plateau.obtenirCase(arrivee);

    // Vérifie que les deux cases existent
    if (caseDepart == nullptr || caseArrivee == nullptr) {
        return false;
    }

    // Vérifie qu'il y a bien une pièce sur la case de départ
    if (!caseDepart->estOccupee()) {
        return false;
    }

    Piece* piece = caseDepart->getPiece();

    // Vérifie que la pièce appartient au joueur courant
    if (piece->getJoueur() != getJoueurCourant()) {
        return false;
    }

    // Calcule les mouvements possibles de la pièce
    std::vector<Position> mouvements = piece->mouvementsPossibles(plateau);
    bool mouvementValide = false;

    // Vérifie que la position d'arrivée fait partie des mouvements possibles
    for (const Position& p : mouvements) {
        if (p.getLigne() == arrivee.getLigne() && p.getColonne() == arrivee.getColonne()) {
            mouvementValide = true;
            break;
        }
    }

    if (!mouvementValide) {
        return false;  // Mouvement interdit pour cette pièce
    }

    // Capture simple si une pièce adverse est sur la case d’arrivée
    if (caseArrivee->estOccupee()) {
        Piece* pieceCapturee = caseArrivee->getPiece();
        if (pieceCapturee->getCouleur() == piece->getCouleur()) {
            return false;  // On ne peut pas capturer ses propres pièces
        }
        // Retirer la pièce capturée du joueur adverse
        Joueur* joueurAdverse = pieceCapturee->getJoueur();
        joueurAdverse->retirerPiece(pieceCapturee);
        caseArrivee->vider();
    }

    // Effectue le déplacement sur le plateau
    plateau.deplacerPiece(depart, arrivee);
    // Passe au joueur suivant
    changerJoueur();

    return true;
}

// Démarre une nouvelle partie : réinitialise l'état et place les pièces
void Jeu::demarrerPartie() {
    etatPartie = EtatPartie::EN_COURS;
    indexJoueurCourant = 0;
    initialiserPieces();
}

// Place les 48 pieces sur le plateau Yalta 12x8 (96 cases)
// Zone BLANC : lignes 0-1  | Zone ROUGE : lignes 6-7  | Zone NOIR : lignes 10-11
void Jeu::initialiserPieces() {

    if (joueurs.size() < 3) return;

    Joueur* jBlanc = joueurs[0];  // BLANC  : zone lignes 0-1
    Joueur* jNoir  = joueurs[1];  // NOIR   : zone lignes 10-11
    Joueur* jRouge = joueurs[2];  // ROUGE  : zone lignes 6-7

    // Fonction locale : ajoute la pièce au joueur ET la pose sur le plateau
    auto placer = [&](Joueur* j, Piece* p) {
        j->ajouterPiece(p);
        plateau.placerPiece(p->getPosition(), p);
    };

    // ===== BLANC (lignes 0-1) =====
    // Ligne 0 : Tour Cavalier Fou Reine Roi Fou Cavalier Tour
    placer(jBlanc, new Tour    (Position(0, 0), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Cavalier(Position(0, 1), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Fou     (Position(0, 2), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Reine   (Position(0, 3), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Roi     (Position(0, 4), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Fou     (Position(0, 5), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Cavalier(Position(0, 6), Couleur::BLANC, jBlanc));
    placer(jBlanc, new Tour    (Position(0, 7), Couleur::BLANC, jBlanc));
    // Ligne 1 : 8 pions blancs (avancent vers la ligne 11, direction +1)
    for (int c = 0; c < 8; c++)
        placer(jBlanc, new Pion(Position(1, c), Couleur::BLANC, jBlanc));

    // ===== ROUGE (lignes 6-7) =====
    // Ligne 7 : Tour Cavalier Fou Reine Roi Fou Cavalier Tour (bord exterieur, rang 3)
// ===== ROUGE (lignes 6-7) =====
// Ligne 7 : bord exterieur
// ===== ROUGE (lignes 6-7) =====
// ===== ROUGE : placement manuel avec indices exacts =====

//// ===== ROUGE : placement manuel exact =====

// Grandes pièces rouges
placer(jRouge, new Tour    (Position(7, 3), Couleur::ROUGE, jRouge)); // T
placer(jRouge, new Cavalier(Position(7, 2), Couleur::ROUGE, jRouge)); // C
placer(jRouge, new Fou     (Position(7, 1), Couleur::ROUGE, jRouge)); // F
placer(jRouge, new Reine   (Position(7, 0), Couleur::ROUGE, jRouge)); // D
placer(jRouge, new Roi     (Position(4, 7), Couleur::ROUGE, jRouge)); // R
placer(jRouge, new Fou     (Position(5, 7), Couleur::ROUGE, jRouge)); // F
placer(jRouge, new Cavalier(Position(6, 7), Couleur::ROUGE, jRouge)); // C
placer(jRouge, new Tour    (Position(7, 7), Couleur::ROUGE, jRouge)); // T

// Pions rouges
placer(jRouge, new Pion(Position(6, 3), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(6, 2), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(6, 1), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(6, 0), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(4, 6), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(5, 6), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(6, 6), Couleur::ROUGE, jRouge));
placer(jRouge, new Pion(Position(7, 6), Couleur::ROUGE, jRouge));


    // ===== NOIR (lignes 10-11) =====
    // Ligne 10 : 8 pions noirs (avancent vers la ligne 0, direction -1)
    for (int c = 0; c < 8; c++)
        placer(jNoir, new Pion(Position(10, c), Couleur::NOIR, jNoir));
    // Ligne 11 : Tour Cavalier Fou Roi Reine Fou Cavalier Tour
    placer(jNoir, new Tour    (Position(11, 0), Couleur::NOIR, jNoir));
    placer(jNoir, new Cavalier(Position(11, 1), Couleur::NOIR, jNoir));
    placer(jNoir, new Fou     (Position(11, 2), Couleur::NOIR, jNoir));
    placer(jNoir, new Reine   (Position(11, 3), Couleur::NOIR, jNoir));
    placer(jNoir, new Roi     (Position(11, 4), Couleur::NOIR, jNoir));
    placer(jNoir, new Fou     (Position(11, 5), Couleur::NOIR, jNoir));
    placer(jNoir, new Cavalier(Position(11, 6), Couleur::NOIR, jNoir));
    placer(jNoir, new Tour    (Position(11, 7), Couleur::NOIR, jNoir));
}

// Fonction utilitaire : recherche le roi parmi les pièces d'un joueur
// Utilise dynamic_cast pour identifier le roi par son type
Piece* trouverRoi(Joueur* joueur) {
    for (Piece* p : joueur->getPieces()) {
        if (dynamic_cast<Roi*>(p) != nullptr) {
            return p;  // Roi trouvé
        }
    }
    return nullptr;  // Pas de roi (ne devrait pas arriver)
}

// Vérifie si le roi d'un joueur est menacé par une pièce adverse (échec)
bool Jeu::estEnEchec(Joueur* joueur) {

    // Trouve le roi du joueur
    Piece* roi = trouverRoi(joueur);

    if (roi == nullptr) {
        return false;  // Pas de roi trouvé
    }

    Position positionRoi = roi->getPosition();

    // Parcourt tous les autres joueurs
    for (Joueur* j : joueurs) {

        if (j == joueur) continue;  // On ne vérifie pas ses propres pièces

        // Parcourt les pièces de chaque adversaire
        for (Piece* p : j->getPieces()) {

            // Calcule les mouvements possibles de chaque pièce adverse
            std::vector<Position> mouvements = p->mouvementsPossibles(plateau);

            // Vérifie si une pièce adverse peut atteindre la position du roi
            for (const Position& pos : mouvements) {
                if (pos.getLigne() == positionRoi.getLigne() &&
                    pos.getColonne() == positionRoi.getColonne()) {
                    return true;  // Le roi est en échec !
                }
            }
        }
    }

    return false;  // Le roi n'est menacé par aucune pièce
}

// Vérifie si un joueur est en échec et mat
// Le joueur est mat s'il est en échec et qu'aucun coup ne peut le sortir de l'échec
bool Jeu::estMat(Joueur* joueur) {
    // Si le joueur n'est pas en échec, il ne peut pas être mat
    if (!estEnEchec(joueur)) {
        return false;
    }

    // Parcourt toutes les pièces du joueur pour tester chaque coup possible
    for (Piece* piece : joueur->getPieces()) {
        std::vector<Position> mouvements = piece->mouvementsPossibles(plateau);

        for (const Position& arrivee : mouvements) {
            Position depart = piece->getPosition();

            Case* caseDepart = plateau.obtenirCase(depart);
            Case* caseArrivee = plateau.obtenirCase(arrivee);

            if (caseDepart == nullptr || caseArrivee == nullptr) {
                continue;
            }

            // Sauvegarder l'état avant simulation
            Piece* pieceCapturee = caseArrivee->getPiece();  // Pièce sur la case d'arrivée (peut être nullptr)
            bool arriveeOccupee = caseArrivee->estOccupee(); // La case était-elle occupée ?

            // Simuler le coup temporairement
            caseDepart->vider();               // Retire la pièce de la case de départ
            caseArrivee->placerPiece(piece);   // Place la pièce sur la case d'arrivée
            piece->setPosition(arrivee);       // Met à jour la position de la pièce

            // Vérifie si le joueur est encore en échec après ce coup
            bool encoreEnEchec = estEnEchec(joueur);

            // Annuler la simulation : remet tout en place
            caseArrivee->vider();              // Vide la case d'arrivée
            caseDepart->placerPiece(piece);    // Remet la pièce au départ
            piece->setPosition(depart);        // Restaure la position

            if (arriveeOccupee) {
                caseArrivee->placerPiece(pieceCapturee);  // Restaure la pièce capturée
            }

            // Si un coup permet de sortir de l'échec, ce n'est pas mat
            if (!encoreEnEchec) {
                return false;
            }
        }
    }

    // Aucun coup ne permet de sortir de l'échec : c'est mat !
    return true;
}

// Ajoute une commande à l'historique des coups (pour pouvoir annuler plus tard)
void Jeu::ajouterCommandeHistorique(CommandeCoup* commande) {
    historiqueCoups.push_back(commande);
}

// Retourne l'historique des coups joués
const std::vector<CommandeCoup*>& Jeu::getHistoriqueCoups() const {
    return historiqueCoups;
}