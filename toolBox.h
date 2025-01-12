//
// Created by Tony Evrard on 12/12/2024.
//

#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdbool.h>

// Couleurs pour l'affichage
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

typedef struct Element {
    struct Element *suivantG;
    struct Element *suivantD;
    struct Element *parent;
    bool visite;
    char event;
} Element;

typedef struct Arbre {
    Element *premier;
    Element *current; // Pointer to the current node
} Arbre;

// tableau associatif pour les evenements, les clés sont les lettres et les valeurs sont les descriptions
typedef struct EventType {
    char letter;             // Les lettres des événements
    const char *description; // Description de l'événement
    int maxOccurrences;      // Nombre maximum d'occurrences de l'événement
} EventType;

// On déclare les eventsTypes
static const EventType eventTypes[] = {
    {'E', "Entrée", 1},
    {'S', "Sortie", 1},
    {'U', "Tu tombes de l'arbre ! Retour au début", 1},
    {'A', "Ton prochain mouvement sera aléatoire", 1},
    {'R', "Tu as perdu la mémoire, tu ne sais plus où tu es", 1},
    {'B', "Il y a beaucoup de brouillard ici ...", 1},
    {'D', "Une porte fermée bloque le passage. Trouve la clé", 1},
    {'K', "Tu as trouvé une clé", 1},
    {'X', "Noeud standard", -1}, // -1 indique qu'il n'y a pas de limite
};

#endif // TOOLBOX_H

