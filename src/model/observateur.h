#pragma once

// Interface du patron de conception Observateur.
// Tout objet souhaitant être notifié des changements d'état du Jeu
// doit hériter de cette classe et implémenter mettreAJour().
class Observateur {
public:
    virtual void mettreAJour() = 0;
    virtual ~Observateur() = default;
};
