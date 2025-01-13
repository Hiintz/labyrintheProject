/**
 * @file toolBox.h
 * @brief Déclarations des fonctions et structures de données pour le jeu du labyrinthe
 *
 * Ce fichier contient les déclarations des fonctions et des structures de données utilisées dans le jeu du labyrinthe.
 * Il inclut les définitions des structures `Element` et `Arbre`, ainsi que les déclarations des fonctions utilitaires.
 *
 * Les structures principales incluent :
 * - Element : Représente un nœud de l'arbre binaire.
 * - Arbre : Représente l'arbre binaire.
 * - EventType : Représente un type d'événement dans le jeu.
 *
 * @note Ce projet a été créé par Tony Evrard le 12/12/2024.
 * @note Utilisation du compilateur GCC.
 */

#ifndef TOOLBOX_H
#define TOOLBOX_H
#include <stdbool.h>

// Couleurs pour l'affichage
#define GREEN "\033[1;32m"
#define RED "\033[1;31m"
#define RESET "\033[0m"

typedef struct Element // Structure d'un élément de l'arbre
{
    struct Element *suivantG; // Pointeur vers le fils gauche
    struct Element *suivantD; // Pointeur vers le fils droit
    struct Element *parent;   // Pointeur vers le parent
    bool visite;              // Indique si le nœud a été visité
    char event;               // Type d'événement associé au nœud
} Element;

typedef struct Arbre // Structure de l'arbre binaire
{
    Element *premier; // Pointeur vers le premier nœud de l'arbre
    Element *current; // Pointeur vers le nœud actuel du joueur
} Arbre;

typedef struct EventType // Structure d'un type d'événement
{
    char letter;             // Les lettres des événements
    const char *description; // Description de l'événement
    int maxOccurrences;      // Nombre maximum d'occurrences de l'événement
} EventType;

// Tableau des événements possibles
static const EventType eventTypes[] = {
    {'E', "Entree", 1},
    {'S', "Sortie", 1},
    {'U', "Tu tombes de l'arbre ! Retour au debut", 1},
    {'A', "Ton prochain mouvement sera aleatoire", 1},
    {'R', "Tu as perdu la memoire, tu ne sais plus ou tu es", 1},
    {'B', "Il y a beaucoup de brouillard ici ...", 1},
    {'D', "Une porte fermee bloque le passage. Trouve la cle", 1},
    {'K', "Tu as trouve une cle", 1},
    {'X', "Noeud standard", -1}, // -1 indique qu'il n'y a pas de limite
};

#endif // TOOLBOX_H
