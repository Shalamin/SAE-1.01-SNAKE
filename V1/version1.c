/**
 * @file snake.c
 * @brief Projet SAE1.01 , faire un snake console en C.
 * @author Keraudren Johan.
 * @version : Version 1.
 * @date : 22/10/2024
 *
 * Le projet de cette SAE1.01 à pour but de réaliser un snake de façon algoritmique dans une console.
 * Le serpent de longueur 10 ira vers la droite pour ce déplacer et A pour quitter .
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#define HAUTEUR_MIN 1     // Hauteur min de spawn du serpent
#define LARGEUR_MIN 1     // Largeur min du spawn du serpent
#define HAUTEUR_MAX 40    // Hauteur max de spawn du serpent
#define LARGEUR_MAX 40    // Largeur max du spawn du serpent
#define TAILLE_SERPENT 10 // taille du serpent
#define TEMPO 100000      // temps en microsecondes. Soit 1 seconde ici.
#define TETE 'O'
#define CORPS 'X'
#define STOP 'a'

void gotoXY(int x, int y)
{
    printf("\033[%d;%df", y, x);
}

int kbhit()
{
    // la fonction retourne :
    // 1 si un caractere est present
    // 0 si pas de caractere present

    int unCaractere = 0;
    struct termios oldt, newt;
    int ch;
    int oldf;

    // mettre le terminal en mode non bloquant
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // restaurer le mode du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        unCaractere = 1;
    }
    return unCaractere;
}

void afficher(int x, int y, char c)
{
    /* @brief Faire l'affichage */

    gotoXY(x, y);
    printf("%c", c);
}
void effacer(int x, int y)
{
    /* @brief On efface la position donner. */

    afficher(x, y, ' ');
}

void dessinerSerpent(int lesX[], int lesY[])
{
    /* @brief On dessine le serpent */
    afficher(lesX[0], lesY[0], TETE);
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        if (lesX[i] < 0)
        {
            lesX[i] = 0;
        }
        if (lesY[i] < 0)
        {
            lesY[i] = 0;
        }

        afficher(lesX[i], lesY[i], CORPS);
    }
    fflush(stdout);
}

void progresser(int lesX[], int lesY[])
{
    /* @brief On efface le dernier caractère puis on déplace le serpent de 1 (modifitaction du tableau de coordonnées)*/

    effacer(lesX[TAILLE_SERPENT - 1], lesY[TAILLE_SERPENT - 1]); // N-1 correspond au dernier anneau du serpent
    /** Explication :
     * On prend N-1 car le tableau va de 0 à [MAX]-1 car comme on part de 0 et pas de 1.
     *	Donc un tableau allant jusqu'a 10 valeurs va enfaite de 0 à 9.
     */

    for (int i = TAILLE_SERPENT - 1; i > 0; i--) // on commence a la fin
    {
        lesX[i] = lesX[i - 1]; // le segment "i" prend la position en X de l'élément précédent "i - 1"
        lesY[i] = lesY[i];
    }
    lesX[0]++; // Déplacer la tête vers la droite
    dessinerSerpent(lesX, lesY);
}

int main()
{
    int x, y;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char touche = 0; // Variable pour stocker la touche appuyée
    do
    {
        // On demande la position de base du serpent.
        printf("Position du snake \nx: ");
        scanf("%d", &x);
        printf("y: ");
        scanf("%d", &y);

        // On test si les valeurs sont comprises entre 1 et 40
        if (x < LARGEUR_MIN || x > LARGEUR_MAX || y < HAUTEUR_MIN || y > HAUTEUR_MAX)
        {
            printf("Coordonnées hors limites !\n");
        }

    } while (x < LARGEUR_MIN || x > LARGEUR_MAX || y < HAUTEUR_MIN || y > HAUTEUR_MAX);

    system("clear");

    // Incrémentation des coordonnées.
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = x--;
        lesY[i] = y--;
    }

    // déplacement du serpent tant que la touche 'a' n'a pas été enfoncer.
    do
    {
        progresser(lesX, lesY);
        usleep(TEMPO);
        if (kbhit())
        {
            touche = getchar(); // Lire la touche pressée
        }
    } while (touche != STOP);

    return EXIT_SUCCESS;
}