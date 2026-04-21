#pragma once

// Tous les états possibles d'une partie
enum class EtatPartie {
    EN_COURS,  // La partie se déroule normalement
    ECHEC,     // Un roi est en train d'être menacé
    MAT,       // Un joueur est mat → il est éliminé
    NULLE,     // Personne ne peut gagner → match nul
    TERMINEE   // La partie est terminée
};