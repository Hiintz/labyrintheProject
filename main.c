#include "toolBox.c"
#include <conio.h>

void main(void) {
    // Seed pour les nombres aléatoires
    srand(time(NULL));

    int choix = 0;
    do{
        printf("\n\n====== Menu ======\n");
        printf("1. Mode Mini (sans event)\n");
        printf("2. Mode Normal (avec event)\n");
        printf("3. Mode Debug (avec event et affichage debug)\n");
        printf("4. Affichage des regles\n");
        printf("5. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                // Mode Mini
                Arbre *arbreMini = init(); // Initialiser l'arbre
                genererArbre(arbreMini, 10); // Générer l'arbre avec un nombre de nœuds donné
                arbreMini->current = arbreMini->premier; // on initialise la position du joueur
                // on clear la console
                system("cls");
                boucleJeu(arbreMini, 0); // on lance la boucle de jeu
                break;
            case 2:
                // Mode Normal
                Arbre *arbre = init(); // Initialiser l'arbre
                genererArbre(arbre, 10); // Générer l'arbre avec un nombre de nœuds donné
                genererEvents(arbre); // Générer les événements
                arbre->current = arbre->premier; // on initialise la position du joueur
                // on clear la console
                system("cls");
                boucleJeu(arbre, 0); // on lance la boucle de jeu
                break;
            case 3:
                Arbre *arbreDebug = init(); // Initialiser l'arbre
                genererArbre(arbreDebug, 10); // Générer l'arbre avec un nombre de nœuds donné
                genererEvents(arbreDebug); // Générer les événements
                arbreDebug->current = arbreDebug->premier; // on initialise la position du joueur
                printf("%d noeu(x) ont ete genere(s)\n\n", nombreElement(arbreDebug->premier));
                // on affiche l'arbre
                boucleJeu(arbreDebug, 1);
                break;
            case 4:
                printf("Affichage des regles\n");
                break;
            case 5:
                printf("Merci et au revoir !\n");
                break;
            default:
                printf("Choix invalide\n");
                break;
        }
    } while (choix != 5);
}