#pragma once

// Interface abstraite représentant un coup dans le jeu (patron de conception Commande)
// Permet d'exécuter et d'annuler n'importe quel type de coup
class CommandeCoup {
public:
    // Exécute le coup (méthode pure virtuelle, doit être implémentée par les sous-classes)
    virtual void executer() = 0;

    // Annule le coup pour revenir à l'état précédent
    virtual void annuler() = 0;

    // Destructeur virtuel par défaut pour permettre la destruction polymorphique
    virtual ~CommandeCoup() = default;
};