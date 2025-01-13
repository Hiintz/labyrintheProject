/**
 * @file toolBox.c
 * @brief Fonctions utilitaires pour le jeu du labyrinthe
 *
 * Ce fichier contient les définitions des fonctions utilitaires utilisées dans le jeu du labyrinthe.
 * Il inclut des fonctions pour initialiser et générer l'arbre binaire, réinitialiser les visites des nœuds,
 * afficher l'arbre de manière graphique, et gérer les événements du jeu.
 *
 * Les fonctions principales incluent :
 * - init : Initialise un nouvel arbre binaire.
 * - genererArbre : Génère un arbre binaire avec un nombre donné de nœuds.
 * - reinitialiserVisite : Réinitialise la visite de tous les nœuds de l'arbre.
 * - afficherArbre : Affiche l'arbre binaire de manière graphique.
 * - afficherArbreJoueur : Affiche l'arbre binaire avec la position actuelle du joueur.
 * - compterNoeudsNonExplores : Compte le nombre de nœuds non explorés dans l'arbre.
 * - afficherReglesDuJeu : Affiche les règles du jeu et les différents événements.
 * - boucleJeu : Boucle principale du jeu.
 *
 * @note Ce projet a été créé par Tony Evrard le 12/12/2024.
 * @note Utilisation du compilateur GCC.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#include "toolBox.h"

/**
 * Initialisation de l'arbre binaire.
 * @return l'arbre binaire initialisé.
 */
Arbre *init()
{
    Arbre *arbre = malloc(sizeof(Arbre)); // Allouer de la mémoire pour l'arbre
    if (arbre == NULL)                    // Vérifier si l'allocation a réussi
    {
        perror("Erreur d'allocation mémoire pour l'arbre");
        exit(EXIT_FAILURE);
    }
    arbre->premier = NULL; // Initialiser le premier nœud à NULL
    return arbre;          // Retourner l'arbre initialisé
}

/**
 * Génération d'un arbre binaire parfait ou non.
 * De façon aléatoire. Avec un nombre de nœuds donné.
 * @param arbre L'arbre binaire à générer.
 * @param nombre Le nombre de nœuds à générer.
 */
void genererArbre(Arbre *arbre, int nombre)
{
    if (arbre->premier == NULL) // Si l'arbre est vide
    {
        Element *nouveau = malloc(sizeof(Element)); // Allouer de la mémoire pour un nouveau nœud
        if (nouveau == NULL)                        // Vérifier si l'allocation a réussi
        {
            perror("Erreur d'allocation mémoire pour un nouveau noeud");
            exit(EXIT_FAILURE);
        }
        nouveau->suivantG = NULL; // Initialiser les pointeurs à NULL
        nouveau->suivantD = NULL;
        nouveau->parent = NULL;
        nouveau->event = 'E';     // Initialiser l'événement à l'entrée
        nouveau->visite = true;   // Marquer l'entrée comme visitée
        arbre->premier = nouveau; // Affecter le premier nœud de l'arbre
        nombre--;                 // Décrémenter le nombre de nœuds restants
    }

    for (int i = 0; i < nombre; i++) // Pour chaque nœud restant à générer
    {
        Element *current = arbre->premier; // Initialiser le nœud actuel à la racine
        while (1)
        {
            // Choisir un enfant aléatoire
            if (rand() % 2 == 0)
            {
                if (current->suivantG == NULL) // On vérifie si le nœud gauche est vide
                {
                    Element *nouveau = malloc(sizeof(Element));
                    if (nouveau == NULL)
                    {
                        perror("Erreur d'allocation mémoire pour un nouveau noeud");
                        exit(EXIT_FAILURE);
                    }
                    nouveau->suivantG = NULL;
                    nouveau->suivantD = NULL;
                    nouveau->parent = current;
                    nouveau->event = 'X';        // Nœud standard pour le moment
                    nouveau->visite = false;     // Nœud non visité
                    current->suivantG = nouveau; // Affecter le nouveau nœud comme enfant gauche
                    break;
                }
                current = current->suivantG; // Passer à l'enfant gauche
            }
            else // même chose mais à droite
            {
                if (current->suivantD == NULL)
                {
                    Element *nouveau = malloc(sizeof(Element));
                    if (nouveau == NULL)
                    {
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
    while (current->suivantG != NULL || current->suivantD != NULL) // Trouver une feuille
    {
        if (current->suivantG != NULL && (current->suivantD == NULL || rand() % 2 == 0)) // Si le nœud gauche existe et le droit n'existe pas ou aléatoirement
        {
            current = current->suivantG;
        }
        else
        {
            current = current->suivantD;
        }
    }
    current->event = 'S'; // Affecter la sortie à la feuille
}

/**
 * Comptage du nombre d'éléments dans l'arbre.
 * @param element L'élément à compter.
 * @return Nombre d'éléments dans l'arbre.
 */
int nombreElement(Element *element)
{
    // Fonction récursive pour compter les éléments
    if (element == NULL)
    {
        return 0;
    }
    return 1 + nombreElement(element->suivantG) + nombreElement(element->suivantD); // Compter l'élément actuel et ses enfants et on retourne le total
}

/**
 * Compte le nombre de nœuds non explorés dans l'arbre.
 * @param element L'élément à partir duquel commencer le comptage.
 * @return Nombre de nœuds non explorés.
 */
int compterNoeudsNonExplores(Element *element)
{
    // fonction récursive pour compter les nœuds non explorés
    if (element == NULL)
    {
        return 0;
    }
    int count = element->visite ? 0 : 1;                                                                      // Si le nœud est visité, on ne le compte pas
    return count + compterNoeudsNonExplores(element->suivantG) + compterNoeudsNonExplores(element->suivantD); // On compte le nœud actuel et ses enfants et on retourne le total
}

/**
 * Collecte tous les nœuds de l'arbre.
 * @param element L'élément à partir duquel commencer la collecte.
 * @param noeud Tableau de nœuds à remplir.
 * @param nbNoeuds Nombre de nœuds collectés.
 */
void collecteNoeuds(Element *element, Element **noeud, int *nbNoeuds)
{
    if (element == NULL)
        return; // Si l'élément est NULL, on ne fait rien
    if (element->event != 'E' && element->event != 'S')
    {                                   // On ne collecte pas les entrées et les sorties
        noeud[(*nbNoeuds)++] = element; // On ajoute le nœud au tableau
    }
    collecteNoeuds(element->suivantG, noeud, nbNoeuds); // On continue la collecte avec les enfants
    collecteNoeuds(element->suivantD, noeud, nbNoeuds);
}

/**
 * Trouver un chemin dans l'arbre.
 * @param noeud Le nœud à partir duquel commencer la recherche.
 * @param cheminNoeuds Tableau de nœuds à remplir.
 * @param longueurChemin Longueur du chemin.
 */
void chercherChemin(Element *noeud, Element **cheminNoeuds, int *longueurChemin)
{
    if (noeud == NULL)
        return; // Si le nœud est NULL, on ne fait rien

    cheminNoeuds[(*longueurChemin)++] = noeud; // On ajoute le nœud au chemin

    if (noeud->suivantG != NULL)
    { // Si le nœud a un enfant gauche, on continue la recherche
        chercherChemin(noeud->suivantG, cheminNoeuds, longueurChemin);
    }
    else if (noeud->suivantD != NULL)
    { // Si le nœud a un enfant droit, on continue la recherche
        chercherChemin(noeud->suivantD, cheminNoeuds, longueurChemin);
    }
    // Si c'est une feuille, ne fait rien
}

/**
 * Génération des événements pour chaque nœud de l'arbre.
 * @param arbre L'arbre binaire à parcourir.
 */
void genererEvents(Arbre *arbre)
{
    // Initialise les compteurs d'événements
    int eventCounts[128] = {0};                               // Pour les 128 caractères ASCII au cas où
    int totalEvents = sizeof(eventTypes) / sizeof(EventType); // Nombre total d'événements enregistrés

    // Extrait les événements déjà placés
    eventCounts['E'] = 1;
    eventCounts['S'] = 1;

    // Flags pour vérifier si la clé et la porte ont été placées
    bool keyOK = false;
    bool doorOK = false;

    // Collecte tous les nœuds de l'arbre
    int maxNoeuds = nombreElement(arbre->premier); // Nombre maximum de nœuds
    Element **nodes = malloc(maxNoeuds * sizeof(Element *));
    if (nodes == NULL)
    {
        perror("Erreur d'allocation mémoire pour les nœuds");
        exit(EXIT_FAILURE);
    }
    int nbNoeuds = 0;
    collecteNoeuds(arbre->premier, nodes, &nbNoeuds); // Collecte les nœuds

    // Assigne 'K' et 'D' à des nœuds aléatoires
    Element *pathNodes[maxNoeuds];
    int longueurChemin = 0;
    chercherChemin(arbre->premier, pathNodes, &longueurChemin);

    Element *keyNode = NULL;

    if (longueurChemin >= 3)
    {
        // on s'assure de placer la clé et la porte entre l'entrée et la sortie
        int keyStartIndex = 1;                // On commence après l'entrée
        int keyEndIndex = longueurChemin - 2; // Et on finit avant la sortie

        if (keyStartIndex <= keyEndIndex)
        {
            // On place 'K' à un nœud aléatoire entre l'entrée et la sortie
            int keyIndex = keyStartIndex + rand() % (keyEndIndex - keyStartIndex + 1);
            keyNode = pathNodes[keyIndex];
            keyNode->event = 'K';
            keyOK = true;
            eventCounts['K']++;

            // On place 'D' à un nœud plus profond que la clé
            Element **deeperNodes = malloc(maxNoeuds * sizeof(Element *));
            if (deeperNodes == NULL)
            {
                perror("Erreur d'allocation mémoire pour les nœuds plus profonds");
                free(nodes);
                exit(EXIT_FAILURE);
            }
            int deeperNodeCount = 0;
            // On collecte les nœuds plus profonds que la clé
            for (int i = 0; i < nbNoeuds; i++)
            {
                if (nodes[i] != keyNode && nodes[i]->parent != NULL)
                {
                    int depth = 0;
                    Element *temp = nodes[i];
                    while (temp != NULL)
                    {
                        depth++;
                        temp = temp->parent;
                    }
                    if (depth > keyIndex)
                    {
                        deeperNodes[deeperNodeCount++] = nodes[i];
                    }
                }
            }

            // On place 'D' à un nœud aléatoire parmi les nœuds plus profonds que la clé
            if (deeperNodeCount > 0)
            {
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
    for (int i = 0; i < nbNoeuds; i++)
    {
        if (nodes[i]->event == 'K' || nodes[i]->event == 'D')
        {
            nodes[i] = nodes[nbNoeuds - 1];
            nbNoeuds--;
            i--;
        }
    }

    // On mélange les nœuds
    for (int i = nbNoeuds - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Element *temp = nodes[i];
        nodes[i] = nodes[j];
        nodes[j] = temp;
    }

    // On assigne les événements restants
    for (int i = 0; i < nbNoeuds; i++)
    {
        Element *node = nodes[i];

        // Liste des événements possibles
        char possibleEvents[128];
        int possibleCount = 0;
        for (int j = 0; j < totalEvents; j++)
        {
            char eventLetter = eventTypes[j].letter;
            int maxOccur = eventTypes[j].maxOccurrences;

            // Exclusion de 'E', 'S', 'K', 'D'
            if (eventLetter == 'E' || eventLetter == 'S' || eventLetter == 'K' || eventLetter == 'D')
                continue;

            if (maxOccur == -1 || eventCounts[(int)eventLetter] < maxOccur)
            {
                possibleEvents[possibleCount++] = eventLetter;
            }
        }

        // On choisit un événement aléatoire parmi les événements possibles
        if (possibleCount > 0)
        {
            char event = possibleEvents[rand() % possibleCount];
            node->event = event;
            eventCounts[(int)event]++;
        }
        else
        {
            node->event = 'X'; // Noeud standard
        }
    }

    free(nodes); // Libère la mémoire allouée pour les nœuds collectés

    // On vérifie que la clé et la porte ont été placées
    if (keyOK && !doorOK)
    {
        printf("Avertissement: Clé générée mais pas de porte. La clé redevient un noeud standard.\n");
        if (keyNode != NULL)
        {
            keyNode->event = 'X';
        }
    }

    if (doorOK && !keyOK)
    {
        printf("Avertissement: Porte générée mais pas de clé.\n");
    }
}

/**
 * Réinitialisation de la visite de tous les nœuds de l'arbre.
 * @param element L'élément à réinitialiser.
 */
void reinitialiserVisite(Element *element)
{
    // Fonction récursive pour réinitialiser la visite de tous les nœuds
    if (element == NULL)
    {
        return;
    }
    element->visite = false;
    reinitialiserVisite(element->suivantG); // Réinitialiser les enfants
    reinitialiserVisite(element->suivantD);
}

/**
 * Affichage de l'arbre binaire de façon graphique pour le débogage.
 * @param element L'élément à afficher.
 * @param profondeur La profondeur de l'élément.
 * @param current L'élément actuel.
 */
void afficherArbre(Element *element, int profondeur, Element *current)
{
    // Indentation pour la profondeur
    for (int i = 0; i < profondeur; i++)
    {
        printf("|   ");
    }
    if (element == NULL)
    {
        printf("|-- NULL\n");
        return;
    }
    if (element == current)
    {
        if (element->visite)
        {
            printf("|-- %c " RED "<- Vous etes ici" RESET "\n", element->event);
        }
        else
        {
            printf("|-- ? " RED "<- Vous etes ici" RESET "\n");
        }
    }
    else if (element == current->parent)
    {
        if (element->visite)
        {
            printf("|-- %c " GREEN "<- Parent" RESET "\n", element->event);
        }
        else
        {
            printf("|-- ? " GREEN "<- Parent" RESET "\n");
        }
    }
    else if (element == current->suivantG)
    {
        if (element->visite)
        {
            printf("|-- %c " GREEN "<- Gauche" RESET "\n", element->event);
        }
        else
        {
            printf("|-- ? " GREEN "<- Gauche" RESET "\n");
        }
    }
    else if (element == current->suivantD)
    {
        if (element->visite)
        {
            printf("|-- %c " GREEN "<- Droite" RESET "\n", element->event);
        }
        else
        {
            printf("|-- ? " GREEN "<- Droite" RESET "\n");
        }
    }
    else
    {
        if (element->visite)
        {
            printf("|-- %c\n", element->event);
        }
        else
        {
            printf("|-- ?\n");
        }
    }

    // On affiche les sous-arbres en commençant par le sous-arbre gauche
    afficherArbre(element->suivantG, profondeur + 1, current);

    // puis le droit
    afficherArbre(element->suivantD, profondeur + 1, current);
}

/**
 * Affichage de l'arbre binaire de façon graphique pour le joueur.
 * @param arbre L'arbre binaire à afficher.
 */
void afficherArbreJoueur(Arbre *arbre)
{
    Element *current = arbre->current;
    printf("\n=============================\n\n");

    // Afficher le parent s'il existe
    if (current->parent != NULL)
    {
        current->parent->visite ? printf("          [%c]\n", current->parent->event) : printf("          [?]\n");
        printf("           |\n");
    }

    // Afficher le nœud actuel
    current->visite ? printf("          [%c] <- Vous etes ici\n", current->event) : printf("          [?] <- Vous etes ici\n");
    printf("         /   \\\n");

    // Afficher les enfants
    if (current->suivantG != NULL)
    {
        current->suivantG->visite ? printf("      [%c]     ", current->suivantG->event) : printf("      [?]     ");
        if (current->suivantD != NULL)
        {
            current->suivantD->visite ? printf("[%c]\n", current->suivantD->event) : printf("[?]\n");
        }
        else
        {
            printf("      \n");
        }
    }
    else if (current->suivantD != NULL)
    {
        current->suivantD->visite ? printf("              [%c]\n", current->suivantD->event) : printf("              [?]\n");
    }
    else
    {
        printf("               \n");
    }

    // si on est à la sortie, on affiche pas les choix possibles
    if (current->event == 'S')
    {
        return;
    }
    // Afficher les choix possibles
    printf("\nChoix possibles:\n");
    int option = 1;
    if (current->suivantG != NULL)
    {
        printf("%d. <- pour aller a gauche \n", option++);
    }
    if (current->suivantD != NULL)
    {
        printf("%d. -> pour aller a droite \n", option++);
    }
    if (current->parent != NULL)
    {
        printf("%d. ^ pour revenir au parent \n", option++);
    }
    printf("%d. Echap pour quitter\n\n", option);
}

/**
 * Affichage des règles du jeu.
 */
void afficherReglesDuJeu()
{
    printf("Bienvenue dans le jeu du labyrinthe !\n");
    printf("Voici les regles du jeu :\n");
    printf("1. Vous commencez a l'entree du labyrinthe (E).\n");
    printf("2. Votre objectif est de trouver la sortie (S).\n");
    printf("3. Vous pouvez vous déplacer vers la gauche ou la droite.\n");
    printf("4. Faites attention aux différents événements que vous pouvez rencontrer :\n");

    // Afficher les différents événements
    for (int i = 0; i < sizeof(eventTypes) / sizeof(eventTypes[0]); i++)
    {
        printf("   - %c : %s\n", eventTypes[i].letter, eventTypes[i].description);
    }

    printf("5. Les noeuds non explorés sont marqués par un point d'interrogation (?).\n");
    printf("6. Bonne chance et amusez-vous bien !\n");
}

/**
 * Boucle de jeu.
 * @param arbre L'arbre binaire à parcourir.
 */
void boucleJeu(Arbre *arbre, int debug)
{
    bool cleTrouvee = false; // pour l'event K et D
    bool randomMove = false; // pour l'event A
    // Boucle du jeu
    while (1)
    {
        randomMove = false; // réinitialisation
        // si on visite pour la première fois la salle R
        if (arbre->current->event == 'R' && arbre->current->visite == false)
        {
            printf("Vous avez perdu la memoire, vous ne savez plus ou vous etes.\n");
            // on réinitialise toutes les salles visitées en les mettant à false
            reinitialiserVisite(arbre->premier);
        }

        // si on visite pour la première fois la salle U
        if (arbre->current->event == 'U' && arbre->current->visite == false)
        {
            printf("Vous tombez de l'arbre ! Retour au debut.\n");
            arbre->current->visite = true;
            arbre->current = arbre->premier;
        }

        // si on visite pour la première fois la clé
        if (arbre->current->event == 'K' && arbre->current->visite == false)
        {
            printf("Vous trouvez une cle ! Mais ou est la porte ?\n");
            cleTrouvee = true;
        }

        // si on trouve la porte pour la première fois
        if (arbre->current->event == 'D' && arbre->current->visite == false)
        {
            if (cleTrouvee)
            {
                printf("Vous tombez sur une porte fermee a cle, et ca tombe bien, vous avez deja la cle !\n");
            }
            else
            {
                printf("Vous tombez sur une porte fermee a cle. Mais ou est la cle pour avancer ?\n");
            }
        }

        // si on visite la salle B
        if (arbre->current->event == 'B')
        {
            printf("Il y a beaucoup de brouillard ici ...\n");
        }

        // si on visite pour la première fois la salle A
        if (arbre->current->event == 'A' && arbre->current->visite == false)
        {
            printf("Tu es desoriente, ton prochain mouvement sera aleatoire.\n");
            randomMove = true;
        }

        // on met le noeud actuel en visité, sauf si c'est un brouillard
        if (arbre->current->event != 'B')
        {
            arbre->current->visite = true;
        }

        // on affiche le nombre de noeuds restants
        printf("\nIl reste " RED "%d" RESET " noeuds non explores\n\n", compterNoeudsNonExplores(arbre->premier));

        if (debug)
        {
            afficherArbre(arbre->premier, 0, arbre->current);
            // on affiche les mouvements possibles
            printf("\nChoix possibles:\n");
            int option = 1;
            if (arbre->current->suivantG != NULL)
            {
                printf("%d. <- pour aller a gauche \n", option++);
            }
            if (arbre->current->suivantD != NULL)
            {
                printf("%d. -> pour aller a droite \n", option++);
            }
            if (arbre->current->parent != NULL)
            {
                printf("%d. ^ pour revenir au parent \n", option++);
            }
            printf("%d. Echap pour quitter\n\n", option);
        }
        else
        {
            afficherArbreJoueur(arbre);
        }
        // si on visite pour la première fois la salle A
        if (randomMove)
        {
            int ch = getch();
            // On choisit un mouvement aléatoire, entre parent, gauche et droite (s'ils existent).
            int i = 0;
            Element *mouvements[3]; // Tableau pour stocker les mouvements possibles
            if (arbre->current->parent != NULL)
            {                                             // Si le parent existe
                mouvements[i++] = arbre->current->parent; // On l'ajoute
            }
            if (arbre->current->suivantG != NULL)
            {                                               // Si le nœud gauche existe
                mouvements[i++] = arbre->current->suivantG; // On l'ajoute
            }
            if (arbre->current->suivantD != NULL)
            {                                               // Si le nœud droit existe
                mouvements[i++] = arbre->current->suivantD; // On l'ajoute
            }
            arbre->current = mouvements[rand() % i]; // On choisit un mouvement aléatoire parmi les mouvements possibles
        }
        else
        {
            printf("Utilisez les fleches pour naviguer, Echap pour quitter.\n\n");
            int ch = getch();
            if (ch == 0 || ch == 224)
            {
                // Touches spéciales (flèches)
                ch = getch();
                switch (ch)
                {
                case 72: // Fleche haut
                    if (arbre->current->parent != NULL)
                    {
                        arbre->current = arbre->current->parent;
                    }
                    else
                    {
                        printf("Pas de parent.\n");
                    }
                    break;
                case 75: // Fleche gauche
                    if (arbre->current->event == 'D' && cleTrouvee == false)
                    {
                        printf("La porte est fermée à cle, vous ne pouvez pas passer.\n");
                    }
                    else if (arbre->current->suivantG != NULL)
                    {
                        arbre->current = arbre->current->suivantG;
                    }
                    else
                    {
                        printf("Pas de noeud gauche.\n");
                    }
                    break;
                case 77: // Fleche droite
                    if (arbre->current->event == 'D' && cleTrouvee == false)
                    {
                        printf("La porte est fermee a cle, vous ne pouvez pas passer.\n");
                    }
                    else if (arbre->current->suivantD != NULL)
                    {
                        arbre->current = arbre->current->suivantD;
                    }
                    else
                    {
                        printf("Pas de noeud droit.\n");
                    }
                    break;
                default:
                    // Autres touches ignorees
                    break;
                }
            }
            else if (ch == 27)
            {
                // Touche Echap pour quitter
                printf("Vous avez quitte le jeu.\n");
                getch(); // Attendre une touche pour quitter
                break;
            }
        }
        // Vérifier si le joueur a atteint la sortie
        if (arbre->current->event == 'S')
        {
            system("cls");
            printf("Felicitations ! Vous avez trouve la sortie.\n");
            arbre->current->visite = true;
            printf("Il restait %d noeuds non explores.\n", compterNoeudsNonExplores(arbre->premier));
            if (debug)
            {
                afficherArbre(arbre->premier, 0, arbre->current);
            }
            else
            {
                afficherArbreJoueur(arbre);
            }
            getch(); // Attendre une touche pour quitter
            system("cls");
            break; // Sortir de la boucle
        }
        // on clear la console
        system("cls");
    }
}
