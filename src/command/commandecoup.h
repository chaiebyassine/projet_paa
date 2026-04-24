#pragma once

// Interface de base pour tous les coups du jeu (patron de conception : Commande)
// Chaque coup peut être exécuté puis annulé
class CommandeCoup {
public:
    virtual void executer() = 0;
    virtual void annuler() = 0;
    virtual bool getExecutionReussie() const = 0;
    virtual ~CommandeCoup() = default;
};