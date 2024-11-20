/**
 * @file serpent_version3_lefrancois_julien.c
 * @brief
 * @author Lefrançois Julien
 * @version version 3
 * @date 10/11/2024
 *
 * le plateau est délimiter et des pavés sont positionné aléatoirement sur le plateau
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <stdbool.h>

/**************************************************************************************************************
 *					           déclaration des constantes et des procédures					        		  *
 ***************************************************************************************************************/

const int TAILLE = 10;
const int DUREE = 100000;
const char TETE = 'O';
const char CORPS = 'X';
const char STOP = 'a';

const char HAUT = 'z';
const char GAUCHE = 'q';
const char BAS = 's';
const char DROITE = 'd';

const char BORDURE = '#';
const char VIDE = ' ';

const int POSITION_X = 40;
const int POSITION_Y = 20;

#define LARGEUR_MAX_PLATEAU 80

#define HAUTEUR_MAX_PLATEAU 40

typedef char tableau[LARGEUR_MAX_PLATEAU + 1][HAUTEUR_MAX_PLATEAU + 1];

void gotoXY(int x, int y);
int kbhit();
void dessinerSerpent(int lesX[], int lesY[]);
void afficher(int x, int y, char c);
void effacer(int x, int y);
void progresser(int lesX[], int lesY[], char touche, bool *collision, tableau plateau);
void enableEcho();
void disableEcho();
void iniPlateau(tableau plateau);
void dessinerPlateau(tableau plateau);

/**************************************************************************************************************
 *								        	     Procédure									        		  *
 ***************************************************************************************************************/

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

void disableEcho()
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

void enableEcho()
{
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1)
    {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1)
    {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief écrit les caractère au coordonnées indiquer
 * @param <x, Entier, Entrée, coordonnée>
 * @param <y, Entier, Entrée, coordonnée>
 */
void gotoXY(int x, int y)
{
    printf("\033[%d;%df", y, x);
}

/**
 * @brief récupère les données des tableaux et les utilises pour dessiner le serpent
 * @param <lesX, Entier, Entrée, tableau>
 * @param <lesY, Entier, Entrée, tableau>
 */
void dessinerSerpent(int lesX[], int lesY[])
{
    /* @brief On dessine le serpent */
    afficher(lesX[0], lesY[0], TETE);
    for (int indice = 1; indice < TAILLE; indice++)
    {
        if (lesX[indice] < 0)
        {
            lesX[indice] = 0;
        }
        if (lesY[indice] < 0)
        {
            lesY[indice] = 0;
        }

        afficher(lesX[indice], lesY[indice], CORPS);
    }
    fflush(stdout);
}

/**
 * @brief récupère les données pour pouvoir afficher le serpent
 * @param <x, Entier, Entrée, coordonnée>
 * @param <y, Entier, Entrée, coordonnée>
 * @param <c, caractère, Entrée, corps du serpent>
 */
void afficher(int x, int y, char c)
{
    gotoXY(x, y);
    printf("%c", c);
}

/**
 * @brief efface le dernier caractère du serpent pour ne pas qu'il s'étale au lieu de se déplacer
 * @param <x, Entier, Entrée, coordonnée>
 * @param <y, Entier, Entrée, coordonnée>
 */
void effacer(int x, int y)
{
    afficher(x, y, ' ');
}

/**
 * @brief fait avancer le serpent
 * @param <lesX, Entier, Entrée, tableau>
 * @param <lesY, Entier, Entrée, tableau>
 * @param <direction, caractère, Entrée, direction choisi>
 */
void progresser(int lesX[], int lesY[], char direction, bool *collision, tableau plateau)
{
    *collision = false;
    /* @brief On efface le dernier caractère puis on déplace le serpent de 1 (modifitaction du tableau de coordonnées)*/
    if (direction == HAUT)
    {
        effacer(lesX[TAILLE - 1], lesY[TAILLE - 1]);        // N-1 correspond au dernier anneau du serpent
        for (int indice = TAILLE - 1; indice > 0; indice--) // on commence a la fin
        {
            lesX[indice] = lesX[indice - 1]; // le segment "i" prend la position en X de l'élément précédent "i - 1"
            lesY[indice] = lesY[indice - 1];
        }
        lesY[0]--;
    }

    if (direction == BAS)
    {
        effacer(lesX[TAILLE - 1], lesY[TAILLE - 1]);        // N-1 correspond au dernier anneau du serpent
        for (int indice = TAILLE - 1; indice > 0; indice--) // on commence a la fin
        {
            lesX[indice] = lesX[indice - 1]; // le segment "i" prend la position en X de l'élément précédent "i - 1"
            lesY[indice] = lesY[indice - 1];
        }
        lesY[0]++;
    }

    if (direction == GAUCHE)
    {
        effacer(lesX[TAILLE - 1], lesY[TAILLE - 1]);        // N-1 correspond au dernier anneau du serpent
        for (int indice = TAILLE - 1; indice > 0; indice--) // on commence a la fin
        {
            lesX[indice] = lesX[indice - 1]; // le segment "i" prend la position en X de l'élément précédent "i - 1"
            lesY[indice] = lesY[indice - 1];
        }
        lesX[0]--;
    }

    if (direction == DROITE)
    {
        effacer(lesX[TAILLE - 1], lesY[TAILLE - 1]);        // N-1 correspond au dernier anneau du serpent
        for (int indice = TAILLE - 1; indice > 0; indice--) // on commence a la fin
        {
            lesX[indice] = lesX[indice - 1]; // le segment "i" prend la position en X de l'élément précédent "i - 1"
            lesY[indice] = lesY[indice - 1];
        }
        lesX[0]++;
    }

    for (int indice = TAILLE - 1; indice > 0; indice--) // on commence a la fin
    {
        if (lesX[0] == lesX[indice] && lesY[0] == lesY[indice])
        {
            *collision = true;
        }

        if (plateau[lesX[0]][lesY[0]] == BORDURE)
        {
            *collision = true;
        }
    }

    // Déplacer la tête vers la droite
    dessinerSerpent(lesX, lesY);
}

void initPlateau(tableau plateau)
{
    for (int lig = 0; lig <= LARGEUR_MAX_PLATEAU; lig++)
    {
        for (int col = 0; col <= HAUTEUR_MAX_PLATEAU; col++)
        {
            if (((lig == 1) || (lig == LARGEUR_MAX_PLATEAU)) || ((col == 1) || (col == HAUTEUR_MAX_PLATEAU)))
            {
                plateau[lig][col] = BORDURE;
            }
            else
            {
                plateau[lig][col] = VIDE;
            }
        }
    }
}

void dessinerPlateau(tableau plateau)
{
    for (int lig = 1; lig <= LARGEUR_MAX_PLATEAU; lig++)
    {
        for (int col = 1; col <= HAUTEUR_MAX_PLATEAU; col++)
        {
            afficher(lig, col, plateau[lig][col]);
        }
    }
}

/**************************************************************************************************************
 *								        	Programme principale											  *
 ***************************************************************************************************************/

int main()
{
    int lesX[TAILLE], lesY[TAILLE];
    char touche = DROITE; // le serpent va vers la droite par défaut
    char ancienneTouche = DROITE;
    bool collision = false;
    int x = POSITION_X;
    tableau plateau;

    initPlateau(plateau);

    // Incrémentation des coordonnées.
    for (int i = 0; i < TAILLE; i++)
    {
        lesX[i] = x--;
        lesY[i] = POSITION_Y;
    }
    dessinerPlateau(plateau);
    // déplacement du serpent tant que la touche 'a' n'a pas été enfoncer.
    do
    {
        disableEcho();
        progresser(lesX, lesY, touche, &collision, plateau);
        ancienneTouche = touche;
        usleep(DUREE);
        enableEcho();
        if (kbhit())
        {
            touche = getchar(); // Lire la touche pressée
        }

        if (touche == DROITE && ancienneTouche == GAUCHE)
        {
            touche = GAUCHE;
        }
        else if (touche == GAUCHE && ancienneTouche == DROITE)
        {
            touche = DROITE;
        }
        else if (touche == HAUT && ancienneTouche == BAS)
        {
            touche = BAS;
        }
        else if (touche == BAS && ancienneTouche == HAUT)
        {
            touche = HAUT;
        }
        else
        {
            if (touche != HAUT && touche != BAS && touche != GAUCHE && touche != DROITE && touche != STOP)
            {
                touche = ancienneTouche;
            }
        }

    } while (touche != STOP && collision == false);
    system("clear");
    return EXIT_SUCCESS;
}