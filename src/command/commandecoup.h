#pragma once

// Interface de base pour tous les coups du jeu (patron de conception : Commande)
// Chaque coup peut être exécuté puis annulé
class CommandeCoup {
public:
    // Exécute le coup
    virtual void executer() = 0;

    // Annule le coup et revient à l'état précédent
    virtual void annuler() = 0;

    virtual ~CommandeCoup() = default;
};