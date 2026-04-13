#include "Jeu.h"
#include "plateau/Case.h"
#include "piece/Piece.h"
#include "piece/Tour.h"
#include "piece/Roi.h"
#include "joueur/Joueur.h"

// Constructeur : crée un jeu avec un plateau de taille donnée
// Le joueur courant est le premier (index 0) et la partie est en cours
Jeu::Jeu(int lignes, int colonnes)
    : plateau(lignes, colonnes), indexJoueurCourant(0), etatPartie(EtatPartie::EN_COURS) {
}

// Ajoute un joueur au jeu (maximum 3 joueurs autorisés)
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
// Initialise les pièces sur le plateau pour les 3 joueurs
// Chaque joueur reçoit une tour et un roi (positions temporaires pour test)
void Jeu::initialiserPieces() {

    // Vérifie qu'il y a bien 3 joueurs
    if (joueurs.size() < 3) {
        return;
    }

    Joueur* j1 = joueurs[0];  // Joueur 1
    Joueur* j2 = joueurs[1];  // Joueur 2
    Joueur* j3 = joueurs[2];  // Joueur 3

    // --- Placement des pièces du joueur 1 (coin haut-gauche) ---
    Tour* tour1 = new Tour(Position(0, 0), j1->getCouleur(), j1);
    Roi* roi1 = new Roi(Position(0, 1), j1->getCouleur(), j1);

    j1->ajouterPiece(tour1);
    j1->ajouterPiece(roi1);

    plateau.placerPiece(Position(0, 0), tour1);
    plateau.placerPiece(Position(0, 1), roi1);

    // --- Placement des pièces du joueur 2 (coin bas-gauche) ---
    Tour* tour2 = new Tour(Position(7, 0), j2->getCouleur(), j2);
    Roi* roi2 = new Roi(Position(7, 1), j2->getCouleur(), j2);

    j2->ajouterPiece(tour2);
    j2->ajouterPiece(roi2);

    plateau.placerPiece(Position(7, 0), tour2);
    plateau.placerPiece(Position(7, 1), roi2);

    // --- Placement des pièces du joueur 3 (coin haut-droite) ---
    Tour* tour3 = new Tour(Position(0, 7), j3->getCouleur(), j3);
    Roi* roi3 = new Roi(Position(0, 6), j3->getCouleur(), j3);

    j3->ajouterPiece(tour3);
    j3->ajouterPiece(roi3);

    plateau.placerPiece(Position(0, 7), tour3);
    plateau.placerPiece(Position(0, 6), roi3);
    
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