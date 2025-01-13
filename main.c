/**
 * @file main.c
 * @brief Jeu du labyrinthe
 *
 * Ce projet implémente un jeu du labyrinthe où le joueur doit naviguer à travers un arbre binaire
 * pour trouver la sortie. Le jeu comporte plusieurs modes, y compris un mode mini sans événements,
 * un mode normal avec des événements, et un mode debug avec des événements et un affichage détaillé.
 *
 * Les touches de direction sont utilisées pour naviguer dans l'arbre, et la touche Echap permet de quitter le jeu.
 *
 * Différents fichiers sont utilisés pour organiser le code :
 * - main.c : le point d'entrée du programme
 * - toolBox.h : les déclarations des fonctions et des structures de données
 * - toolBox.c : les définitions des fonctions
 *
 * Les événements incluent :
 * - 'E' : Entrée du labyrinthe
 * - 'S' : Sortie du labyrinthe
 * - 'U' : Tu tombes de l'arbre ! Retour au début
 * - 'A' : Ton prochain mouvement sera aléatoire
 * - 'R' : Tu as perdu la mémoire, tu ne sais plus où tu es
 * - 'B' : Il y a beaucoup de brouillard ici ...
 * - 'D' : Une porte fermée bloque le passage. Trouve la clé
 * - 'K' : Tu as trouvé une clé
 * - 'X' : Nœud standard
 *
 * @note Ce projet a été créé par Tony Evrard le 12/12/2024.
 * @note Utilisation du compilateur GCC.
 */

#include "toolBox.c"
#include <conio.h>

void main(void)
{
    // Seed pour les nombres aléatoires
    srand(time(NULL));

    // Taille de l'arbre
    int tailleArbre = 10;

    int choix = 0;
    do
    {
        printf("\n\n====== Menu ======\n");
        printf("1. Mode Mini (sans event)\n");
        printf("2. Mode Normal (avec event)\n");
        printf("3. Mode Debug (avec event et affichage debug)\n");
        printf("4. Affichage des regles\n");
        printf("5. Changer la taille de l'arbre (actuellement %d)\n", tailleArbre);
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch (choix)
        {
        case 1:
            // Mode Mini
            Arbre *arbreMini = init();               // Initialiser l'arbre
            genererArbre(arbreMini, tailleArbre);    // Générer l'arbre avec un nombre de nœuds donné
            arbreMini->current = arbreMini->premier; // on initialise la position du joueur
            // on clear la console
            system("cls");
            boucleJeu(arbreMini, 0); // on lance la boucle de jeu
            break;
        case 2:
            // Mode Normal
            Arbre *arbre = init();            // Initialiser l'arbre
            genererArbre(arbre, tailleArbre); // Générer l'arbre avec un nombre de nœuds donné
            genererEvents(arbre);             // Générer les événements
            arbre->current = arbre->premier;  // on initialise la position du joueur
            // on clear la console
            system("cls");
            boucleJeu(arbre, 0); // on lance la boucle de jeu
            break;
        case 3:
            Arbre *arbreDebug = init();                // Initialiser l'arbre
            genererArbre(arbreDebug, tailleArbre);     // Générer l'arbre avec un nombre de nœuds donné
            genererEvents(arbreDebug);                 // Générer les événements
            arbreDebug->current = arbreDebug->premier; // on initialise la position du joueur
            printf("%d noeu(x) ont ete genere(s)\n\n", nombreElement(arbreDebug->premier));
            // on affiche l'arbre
            boucleJeu(arbreDebug, 1);
            break;
        case 4:
            afficherReglesDuJeu(); // Afficher les règles du jeu
            break;
        case 5:
            int taille = 0;
            while (taille < 10 || taille > 50)
            {
                printf("Entrez la taille de l'arbre : (entre 10 et 50) ");
                scanf("%d", &taille);
            }
            tailleArbre = taille;
            break;
        case 6:
            printf("Merci et au revoir !\n");
            break;
        default:
            printf("Choix invalide\n");
            break;
        }
    } while (choix != 6);
}