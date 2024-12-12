//
// Created by Tony Evrard on 12/12/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "toolBox.h"

/**
 * Initialisation de l'arbre binaire.
 * @return l'arbre binaire initialisé.
 */
Arbre *init() {
    Arbre *arbre = malloc(sizeof(Arbre));
    if (arbre == NULL) {
        perror("Erreur d'allocation mémoire pour l'arbre");
        exit(EXIT_FAILURE);
    }
    arbre->premier = NULL;
    return arbre;
}

/**
 * Génération d'un arbre binaire parfait ou non.
 * De façon aléatoire. Avec un nombre de nœuds donné.
 * @param arbre L'arbre binaire à générer.
 * @param nombre Le nombre de nœuds à générer.
 */
void genererArbre(Arbre *arbre, int nombre) {
    if (arbre->premier == NULL) {
        Element *nouveau = malloc(sizeof(Element));
        if (nouveau == NULL) {
            perror("Erreur d'allocation mémoire pour un nouveau noeud");
            exit(EXIT_FAILURE);
        }
        nouveau->suivantG = NULL;
        nouveau->suivantD = NULL;
        nouveau->parent = NULL;
        nouveau->event = 'E';
        nouveau->visite = true;
        arbre->premier = nouveau;
        nombre--;
    }

    for (int i = 0; i < nombre; i++) {
        Element *current = arbre->premier;
        while (1) {
            if (rand() % 2 == 0) {
                if (current->suivantG == NULL) {
                    Element *nouveau = malloc(sizeof(Element));
                    if (nouveau == NULL) {
                        perror("Erreur d'allocation mémoire pour un nouveau noeud");
                        exit(EXIT_FAILURE);
                    }
                    nouveau->suivantG = NULL;
                    nouveau->suivantD = NULL;
                    nouveau->parent = current;
                    nouveau->event = 'X';
                    nouveau->visite = false;
                    current->suivantG = nouveau;
                    break;
                }
                current = current->suivantG;
            } else {
                if (current->suivantD == NULL) {
                    Element *nouveau = malloc(sizeof(Element));
                    if (nouveau == NULL) {
                        perror("Erreur d'allocation mémoire pour un nouveau noeud");
                        exit(EXIT_FAILURE);
                    }
                    nouveau->suivantG = NULL;
                    nouveau->suivantD = NULL;
                    nouveau->parent = current;
                    nouveau->event = 'X';
                    nouveau->visite = false;
                    current->suivantD = nouveau;
                    break;
                }
                current = current->suivantD;
            }
        }
    }

    // Ajout de la sortie dans une feuille aléatoire
    Element *current = arbre->premier;
    while (current->suivantG != NULL || current->suivantD != NULL) {
        if (current->suivantG != NULL && (current->suivantD == NULL || rand() % 2 == 0)) {
            current = current->suivantG;
        } else {
            current = current->suivantD;
        }
    }
    current->event = 'S';
}

/**
 * Comptage du nombre d'éléments dans l'arbre.
 * @param element L'élément à compter.
 * @return Nombre d'éléments dans l'arbre.
 */
int nombreElement(Element *element) {
    if (element == NULL) {
        return 0;
    }
    return 1 + nombreElement(element->suivantG) + nombreElement(element->suivantD);
}

/**
 * Collecte tous les nœuds de l'arbre.
 * @param element L'élément à partir duquel commencer la collecte.
 * @param nodes Tableau de nœuds à remplir.
 * @param nodeCount Nombre de nœuds collectés.
 */
void collecteNoeuds(Element *element, Element **noeud, int *nbNoeuds) {
    if (element == NULL) return; // Si l'élément est NULL, on ne fait rien
    if (element->event != 'E' && element->event != 'S') { // On ne collecte pas les entrées et les sorties
        noeud[(*nbNoeuds)++] = element; // On ajoute le nœud au tableau
    }
    collecteNoeuds(element->suivantG, noeud, nbNoeuds); // On continue la collecte avec les enfants
    collecteNoeuds(element->suivantD, noeud, nbNoeuds);
}

/**
 * Trouver un chemin dans l'arbre.
 * @param node Le nœud à partir duquel commencer la recherche.
 * @param pathNodes Tableau de nœuds à remplir.
 * @param pathLength Longueur du chemin.
 */
void chercherChemin(Element *noeud, Element **cheminNoeuds, int *longueurChemin) {
    if (noeud == NULL) return; // Si le nœud est NULL, on ne fait rien

    cheminNoeuds[(*longueurChemin)++] = noeud; // On ajoute le nœud au chemin

    if (noeud->suivantG != NULL) { // Si le nœud a un enfant gauche, on continue la recherche
        chercherChemin(noeud->suivantG, cheminNoeuds, longueurChemin);
    } else if (noeud->suivantD != NULL) { // Si le nœud a un enfant droit, on continue la recherche
        chercherChemin(noeud->suivantD, cheminNoeuds, longueurChemin);
    }
    // Si c'est une feuille, ne fait rien
}

/**
 * Génération des événements pour chaque nœud de l'arbre.
 * @param arbre L'arbre binaire à parcourir.
 */
void genererEvents(Arbre *arbre) {
    // Initialise les compteurs d'événements
    int eventCounts[128] = {0}; // Pour les 128 caractères ASCII
    int totalEvents = sizeof(eventTypes) / sizeof(EventType);

    // Extrait les événements déjà placés
    eventCounts['E'] = 1;
    eventCounts['S'] = 1;

    // Flags pour vérifier si la clé et la porte ont été placées
    bool keyOK = false;
    bool doorOK = false;

    // Collecte tous les nœuds de l'arbre
    int maxNoeuds = nombreElement(arbre->premier);
    Element **nodes = malloc(maxNoeuds * sizeof(Element*));
    if (nodes == NULL) {
        perror("Erreur d'allocation mémoire pour les nœuds");
        exit(EXIT_FAILURE);
    }
    int nbNoeuds = 0;
    collecteNoeuds(arbre->premier, nodes, &nbNoeuds);

    // Assigne 'K' et 'D' à des nœuds aléatoires
    Element *pathNodes[maxNoeuds];
    int longueurChemin = 0;
    chercherChemin(arbre->premier, pathNodes, &longueurChemin);

    Element *keyNode = NULL;

    if (longueurChemin >= 3) {
        // on s'assure de placer la clé et la porte entre l'entrée et la sortie
        int keyStartIndex = 1; // Start from the node after the entrance
        int keyEndIndex = longueurChemin - 2; // End before the exit node

        if (keyStartIndex <= keyEndIndex) {
            // Place 'K' at a random node between entrance and exit
            int keyIndex = keyStartIndex + rand() % (keyEndIndex - keyStartIndex + 1);
            keyNode = pathNodes[keyIndex];
            keyNode->event = 'K';
            keyOK = true;
            eventCounts['K']++;

            // Collect all nodes at a depth greater than the key node
            Element **deeperNodes = malloc(maxNoeuds * sizeof(Element*));
            if (deeperNodes == NULL) {
                perror("Erreur d'allocation mémoire pour les nœuds plus profonds");
                free(nodes);
                exit(EXIT_FAILURE);
            }
            int deeperNodeCount = 0;
            for (int i = 0; i < nbNoeuds; i++) {
                if (nodes[i] != keyNode && nodes[i]->parent != NULL) {
                    int depth = 0;
                    Element *temp = nodes[i];
                    while (temp != NULL) {
                        depth++;
                        temp = temp->parent;
                    }
                    if (depth > keyIndex) {
                        deeperNodes[deeperNodeCount++] = nodes[i];
                    }
                }
            }

            // Place 'D' at a random node from the deeper nodes
            if (deeperNodeCount > 0) {
                int doorIndex = rand() % deeperNodeCount;
                Element *doorNode = deeperNodes[doorIndex];
                doorNode->event = 'D';
                doorOK = true;
                eventCounts['D']++;
            }

            free(deeperNodes);
        }
    }

    // On retire les nœuds avec 'K' et 'D' de la liste
    for (int i = 0; i < nbNoeuds; i++) {
        if (nodes[i]->event == 'K' || nodes[i]->event == 'D') {
            nodes[i] = nodes[nbNoeuds - 1];
            nbNoeuds--;
            i--;
        }
    }

    // On mélange les nœuds
    for (int i = nbNoeuds - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Element *temp = nodes[i];
        nodes[i] = nodes[j];
        nodes[j] = temp;
    }

    // On assigne les événements restants
    for (int i = 0; i < nbNoeuds; i++) {
        Element *node = nodes[i];

        // Liste des événements possibles
        char possibleEvents[128];
        int possibleCount = 0;
        for (int j = 0; j < totalEvents; j++) {
            char eventLetter = eventTypes[j].letter;
            int maxOccur = eventTypes[j].maxOccurrences;

            // Exclusion de 'E', 'S', 'K', 'D'
            if (eventLetter == 'E' || eventLetter == 'S' || eventLetter == 'K' || eventLetter == 'D')
                continue;

            if (maxOccur == -1 || eventCounts[(int)eventLetter] < maxOccur) {
                possibleEvents[possibleCount++] = eventLetter;
            }
        }

        // On choisit un événement aléatoire parmi les événements possibles
        if (possibleCount > 0) {
            char event = possibleEvents[rand() % possibleCount];
            node->event = event;
            eventCounts[(int)event]++;
        }
    }

    free(nodes);

    // On vérifie que la clé et la porte ont été placées
    if (keyOK && !doorOK) {
        printf("Avertissement: Clé générée mais pas de porte. La clé redevient un noeud standard.\n");
        if (keyNode != NULL) {
            keyNode->event = 'X';
        }
    }

    if (doorOK && !keyOK) {
        printf("Avertissement: Porte générée mais pas de clé.\n");
    }
}

/**
 * Affichage de l'arbre binaire de façon graphique.
 * @param element L'élément à afficher.
 * @param profondeur La profondeur de l'élément.
 */
void afficherArbre(Element *element, int profondeur) {
    // Indentation pour la profondeur
    for (int i = 0; i < profondeur; i++) {
        printf("|   ");
    }
    if (element == NULL) {
        printf("|-- NULL\n");
        return;
    }
    printf("|-- %c\n", element->event);

    // On affiche les sous-arbres en commençant par le sous-arbre gauche
    afficherArbre(element->suivantG, profondeur + 1);

    // puis le droit
    afficherArbre(element->suivantD, profondeur + 1);
}

/**
    * Affichage de l'arbre binaire de façon graphique pour le joueur.
    * @param arbre L'arbre binaire à afficher.
    */
void afficherArbreJoueur(Arbre *arbre) {
    Element *current = arbre->current;
    printf("\n=============================\n");

    // Afficher le parent s'il existe
    if (current->parent != NULL) {
        printf("          [%c]\n", current->parent->event);
        printf("           |\n");
    }

    // Afficher le nœud actuel
    printf("          [%c] <- Vous etes ici\n", current->event);
    printf("         /   \\\n");

    // Afficher les enfants
    if (current->suivantG != NULL) {
        current->suivantG->visite ? printf("      [%c]     ", current->suivantG->event) : printf("      [?]     ");
        if (current->suivantD != NULL) {
            current->suivantD->visite ? printf("[%c]\n", current->suivantD->event) : printf("[?]\n");
        } else {
            printf("      \n");
        }
    } else if (current->suivantD != NULL) {
        printf("              [%c]\n", current->suivantD->event);
    } else {
        printf("               \n");
    }

    // Afficher les choix possibles
    printf("\nChoix possibles:\n");
    int option = 1;
    if (current->suivantG != NULL) {
        printf("%d. <- pour aller a gauche \n", option++);
    }
    if (current->suivantD != NULL) {
        printf("%d. -> pour aller a droite \n", option++);
    }
    if (current->parent != NULL) {
        printf("%d. ^ pour revenir au parent \n", option++, current->parent->event);
    }
    printf("%d.Echap pour quitter\n\n", option);
}

/**
 * Boucle de jeu.
 * @param arbre L'arbre binaire à parcourir.
 */
void boucleJeu(Arbre *arbre) {
    // Boucle du jeu
    while (1) {
        // on met le noeud actuel en visité
        arbre->current->visite = true;
        afficherArbreJoueur(arbre);
        printf("Utilisez les fleches pour naviguer, Echap pour quitter.\n\n");

        int ch = getch();
        if (ch == 0 || ch == 224) {
            // Touches spéciales (flèches)
            ch = getch();
            switch (ch) {
                case 72: // Fleche haut
                    if (arbre->current->parent != NULL) {
                        arbre->current = arbre->current->parent;
                    } else {
                        printf("Pas de parent.\n");
                    }
                    break;
                case 75: // Fleche gauche
                    if (arbre->current->suivantG != NULL) {
                        arbre->current = arbre->current->suivantG;
                    } else {
                        printf("Pas de noeud gauche.\n");
                    }
                    break;
                case 77: // Fleche droite
                    if (arbre->current->suivantD != NULL) {
                        arbre->current = arbre->current->suivantD;
                    } else {
                        printf("Pas de noeud droit.\n");
                    }
                    break;
                default:
                    // Autres touches ignorees
                    break;
            }
        } else if (ch == 27) {
            // Touche Echap pour quitter
            printf("Vous avez quitte le jeu.\n");
            break;
        }
        // Vérifier si le joueur a atteint la sortie
        if (arbre->current->event == 'S') {
            printf("Felicitations ! Vous avez trouve la sortie.\n");
            break;
        }
    }
}

