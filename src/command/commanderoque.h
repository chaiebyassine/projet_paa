#pragma once

#include "commandecoup.h"
#include "../model/jeu.h"
#include "../model/base/position.h"

class Roi;
class Tour;

// Commande du roque : déplace simultanément le Roi et la Tour.
// Le Roi avance de 2 cases vers la Tour, la Tour saute de l'autre côté.
// Les positions d'arrivée sont calculées par l'appelant (ControleurJeu).
// Patron Commande : executer() / annuler() gèrent les deux pièces ensemble.
class CommandeRoque : public CommandeCoup {
private:
    Jeu*     jeu;
    Roi*     roi;
    Tour*    tour;
    Position posRoiDepart;
    Position posRoiArrivee;
    Position posTourDepart;
    Position posTourArrivee;
    bool     executionReussie;
    bool     roiAvaitBouge;    // sauvegardé pour restaurer le flag lors de l'annulation
    bool     tourAvaitBouge;

public:
    // posRoiArrivee  : case de destination du Roi (2 pas vers la Tour)
    // posTourArrivee : case de destination de la Tour (de l'autre côté du Roi)
    CommandeRoque(Jeu* jeu, Roi* roi, Tour* tour,
                  const Position& posRoiArrivee,
                  const Position& posTourArrivee);

    // Déplace Roi puis Tour si le roque est légal
    void executer() override;

    // Remet Roi et Tour à leurs cases de départ, restaure leurs flags aDejaBouge
    void annuler() override;

    bool getExecutionReussie() const override;

    // Vérifie les trois conditions du roque :
    // 1) ni le Roi ni la Tour n'ont bougé
    // 2) le Roi n'est pas en échec
    // 3) toutes les cases entre eux sont vides
    static bool peutRoquer(Jeu& jeu, Roi* roi, Tour* tour);
};
