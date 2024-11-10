/**
 * @file snake.touche
 * @brief Projet SAE1.01 , faire un snake console en C.
 * @author Keraudren Johan.
 * @version : Version 2
 * @date : 29/10/2024
 *
 * Le projet de cette SAE1.01 à pour but de réaliser un snake de façon algoritmique dans une console.
 * Le serpent de longueur 10 ira vers la droite , se deplace en ZQSD , stop avec A .
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

// taille
#define HAUTEUR_MIN 0  // Hauteur max de spawn du serpent
#define LARGEUR_MIN 0  // Largeur max du spawn du serpent
#define HAUTEUR_MAX 40 // Hauteur max de spawn du serpent
#define LARGEUR_MAX 40 // Largeur max du spawn du serpent
// Apparition par défaut
#define X_INITITAL 20 // Position de base en X du serpent
#define Y_INITIAL 20  // Position de base en Y du serpent

// Serpent
#define TETE 'O'
#define CORPS 'X'
#define TAILLE_SERPENT 10          // taille du serpent
#define VITESSE_DEPLACEMENT 100000 // temps en microsecondes. Soit 1 seconde ici.

// Deplacement
#define HAUT 'z'
#define BAS 's'
#define GAUCHE 'q'
#define DROITE 'd'
#define STOP 'a'

/** Définition des procédures */
void afficher(int x, int y, char c);
void effacer(int x, int y);
char definirDirection(char touche, char diection);
void dessinerSerpent(int lesX[], int lesY[]);
void progresser(int lesX[], int lesY[], char direction);
void finDuJeu();

/** Boite à outils */
void gotoXY(int x, int y);
int kbhit(void);
void disableEcho();
void enableEcho();

/*****************************************************
 *                 PROGRAMME PRINCIPALE              *
 *****************************************************/
int main()
{
    // Initialisation des variables .
    int x, y;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char touche = DROITE;    // Variable pour stocker la touche appuyée && mise a DROITE pour que le serpent va vers la droite
    char direction = DROITE; // Variable pour définir la direction

    system("clear");
    disableEcho();

    // Incrémentation des coordonnées.
    x = X_INITITAL;
    y = Y_INITIAL;
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = x--;
        lesY[i] = y;
    }

    // déplacement du serpent tant que la touche 'a' n'a pas été enfoncer.
    do
    {
        if (kbhit())
        {
            touche = getchar(); // Lire la touche pressée
        }
        direction = definirDirection(touche, direction);
        progresser(lesX, lesY, direction);
        usleep(VITESSE_DEPLACEMENT);
    } while (touche != STOP);
    finDuJeu();
    return EXIT_SUCCESS;
}

void afficher(int x, int y, char c)
{
    /* @brief Faire l'affichage */
    if (((x > LARGEUR_MIN) && (x < LARGEUR_MAX)) && ((y > HAUTEUR_MIN) && (y < HAUTEUR_MAX))) // check pour savoir si la valeur a écrire se situe dans l'espace de jeu
    {
        gotoXY(x, y);
        printf("%c", c);
    }
}

void effacer(int x, int y)
{
    /* @brief On efface la position donner. */

    afficher(x, y, ' ');
}

char definirDirection(char touche, char direction)
{
    /* @brief Fonction permettant de modifier la direction de déplacement du serpent .*/

    if (touche == HAUT && direction != BAS) // Récupération de la nouvelle valeur et de l'ancienne
    {
        direction = HAUT; // assignation
    }
    else if ((touche == BAS) && (direction != HAUT)) // Récupération de la nouvelle valeur et de l'ancienne
    {
        direction = BAS; // assignation
    }
    else if ((touche == GAUCHE) && (direction != DROITE)) // Récupération de la nouvelle valeur et de l'ancienne
    {
        direction = GAUCHE; // assignation
    }
    else if ((touche == DROITE) && (direction != GAUCHE)) // Récupération de la nouvelle valeur et de l'ancienne
    {
        direction = DROITE; // assignation
    }
    return direction;
}

void dessinerSerpent(int lesX[], int lesY[])
{
    /* @brief On dessine le serpent */

    afficher(lesX[0], lesY[0], TETE);
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        afficher(lesX[i], lesY[i], CORPS);
    }
    fflush(stdout);
}

void progresser(int lesX[], int lesY[], char direction)
{
    /* @brief On efface le dernier caractère puis on déplace le serpent de 1 (modifitaction du tableau de coordonnées)*/

    effacer(lesX[TAILLE_SERPENT - 1], lesY[TAILLE_SERPENT - 1]); // TAILLE_SERPENT-1 correspond au dernier anneau du serpent
                                                                 /** Explication :
                                                                  * On prend TAILLE_SERPENT-1 car le tableau va de 0 à [MAX]-1 car comme on part de 0 et pas de 1.
                                                                  *	Donc un tableau allant jusqu'a 10 valeurs va enfaite de 0 à 9.
                                                                  */
    for (int i = TAILLE_SERPENT - 1; i > 0; i--)                 // on commence a la fin
    {
        lesX[i] = lesX[i - 1]; // le segment "indice" prend la position en X de l'élément précédent "indice - 1"
        lesY[i] = lesY[i - 1];
    }
    // DROITE:
    if (direction == DROITE)
    {
        lesX[0]++; // Déplacer la tête vers la droite
    }
    else if (direction == GAUCHE)
    {
        lesX[0]--; // Déplacer la tête vers la gauche
    }
    else if (direction == HAUT)
    {
        lesY[0]--; // Déplacer la tête vers le haut (max console)
    }
    else if (direction == BAS)
    {
        lesY[0]++; // Déplacer la tête vers le bas (vers 1)
    }

    dessinerSerpent(lesX, lesY);
}
void finDuJeu()
{
    /* @brief Fin du programme , message de fin et réactivation de l'écriture dans la console*/
    enableEcho();
    system("clear");
    printf("La partie est terminié !");
}
/*****************************************************
 *                 BOITES A OUTILS                   *
 *****************************************************/
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