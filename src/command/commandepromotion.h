#pragma once

#include "commandecoup.h"
#include "../model/jeu.h"

class Pion;
class Piece;

// Choix de la pièce vers laquelle un Pion peut être promu
enum class TypePromotion { REINE, TOUR, FOU, CAVALIER };

// Commande de promotion : remplace un Pion arrivé en zone adverse
// par la pièce de son choix (Reine par défaut).
// L'annulation restaure le Pion et supprime la pièce promue.
class CommandePromotion : public CommandeCoup {
private:
    Jeu*          jeu;
    Pion*         pion;             // le pion à promouvoir (conservé pour pouvoir annuler)
    TypePromotion typePromotion;
    Piece*        piecePromue;      // nouvelle pièce créée lors de executer()
    bool          executionReussie;

public:
    // Par défaut, le pion est promu en Reine (cas le plus courant)
    CommandePromotion(Jeu* jeu, Pion* pion,
                      TypePromotion type = TypePromotion::REINE);

    // Remplace le pion par la pièce promue sur le plateau et dans la liste du joueur
    void executer() override;

    // Remet le pion en place et détruit la pièce promue
    void annuler() override;

    bool getExecutionReussie() const override;

    // Retourne la pièce créée après une exécution réussie (nullptr sinon)
    Piece* getPiecePromue() const { return piecePromue; }

private:
    // Instancie la pièce correspondant au TypePromotion choisi
    Piece* creerPiecePromue() const;
};
