#pragma once

// Énumération représentant les différents états possibles d'une partie d'échecs
enum class EtatPartie {
    EN_COURS,  // La partie est en cours de jeu
    ECHEC,     // Un joueur est en échec
    MAT,       // Un joueur est en échec et mat (fin de partie)
    NULLE,     // La partie est nulle (match nul)
    TERMINEE   // La partie est terminée
};