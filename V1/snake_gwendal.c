/**
 * \author Duvernet--Le Bras Gwendal
 * \brief programme Snake
 * \version 1.1
 * \date 25 octobre 2025
 *
 * Ce programme permet au serpent de se deplacer vers la droite
 * tant que l'utilisateur n'a pas appuyé sur la touche 'a'.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>

#define N 10        // Longueur du serpent
#define TEMPO 50000 // Temporisation en microsecondes
#define MAX_X 40    // Largeur de la zone de jeu
#define MAX_Y 20    // Hauteur de la zone de jeu
const char TETE = 'O', CORPS = 'X', RUPTURE = 'a';

// Fonction pour positionner le curseur
void gotoXY(int x, int y)
{
    printf("\033[%d;%df", y, x);
}

// Fonction pour détecter la frappe d'une touche
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

// Fonction pour afficher un caractère à une position donnée
void afficher(int x, int y, char c)
{
    gotoXY(x, y);
    putchar(c);
}

// Fonction pour effacer un caractère à une position donnée
void effacer(int x, int y)
{
    afficher(x, y, ' ');
}

// Fonction pour dessiner le serpent à l'écran
void dessinerSerpent(int lesX[], int lesY[])
{
    for (int i = 0; i < N - 1; i++)
    {
        afficher(lesX[i], lesY[i], CORPS);
    }
    afficher(lesX[N - 1], lesY[N - 1], TETE);
}

// Fonction pour faire progresser le serpent
void progresser(int lesX[], int lesY[])
{
    effacer(lesX[0], lesY[0]); // Efface l'ancienne position de la queue

    // Décale chaque segment vers la droite
    for (int i = 0; i < N - 1; i++)
    {
        lesX[i] = lesX[i + 1];
        lesY[i] = lesY[i];
    }

    // Déplace la tête
    lesX[N - 1]++;
    if (lesX[N - 1] > MAX_X)
        lesX[N - 1] = 1; // Réinitialise la tête si elle dépasse la limite

    dessinerSerpent(lesX, lesY); // Redessine le serpent
}

// Fonction principale
int main()
{
    int lesX[N], lesY[N];
    int x_initial, y_initial;
    char stop = 0;

    // Initialisation de la position du serpent
    printf("Entrez la position initiale du serpent (x entre 1 et %d) : ", MAX_X);
    scanf("%d", &x_initial);

    // Vérifie que les valeurs entrées sont valides
    if (x_initial < 1 || x_initial > MAX_X)
    {
        printf("Position invalide. La coordonée en x n'est pas correct.\n");
        do
        {
            printf("Entrez la position initiale du serpent (x entre 1 et %d) : ", MAX_X);
            scanf("%d", &x_initial);
        } while (x_initial < 1 || x_initial > MAX_X);
    }

    printf("Entrez la position initiale du serpent (y entre 1 et %d) : ", MAX_Y);
    scanf("%d", &y_initial);
    // Vérifie que les valeurs entrées sont valides
    if (y_initial < 1 || y_initial > MAX_Y)
    {
        printf("Position invalide. La coordonée en y n'est pas correct.\n");
        do
        {
            printf("Entrez la position initiale du serpent (y entre 1 et %d) : ", MAX_Y);
            scanf("%d", &y_initial);
        } while (y_initial < 1 || y_initial > MAX_Y);
    }

    // Positionne la tête et les segments du corps du serpent
    for (int i = 0; i < N; i++)
    {
        lesX[i] = x_initial - i;
        lesY[i] = y_initial;
    }

    system("clear");

    // Boucle principale du jeu
    while (stop != RUPTURE)
    { // Arrête le programme si une touche est pressée
        if (kbhit())
        {
            stop = getchar();
        }
        progresser(lesX, lesY);
        usleep(TEMPO); // Temporisation
    }

    return EXIT_SUCCESS;
}
