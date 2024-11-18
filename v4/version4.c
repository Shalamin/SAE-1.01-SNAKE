/**
 * @file snake.c
 * @brief Projet SAE1.01 - Jeu Snake en console.
 *
 * Ce programme implémente un jeu Snake en console, avec des fonctionnalités de déplacement, gestion des collisions,
 * génération de pavés et affichage dynamique.
 *
 * @author Keraudren Johan
 * @version 3.0
 * @date 11/11/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <time.h>

/** @defgroup Constantes Constantes du jeu */
/**@{*/

/** @brief Largeur minimale de l'aire de jeu */
#define LARGEUR_MIN 1
/** @brief Hauteur minimale de l'aire de jeu */
#define HAUTEUR_MIN 1
/** @brief Largeur maximale de l'aire de jeu */
#define LARGEUR_MAX 80
/** @brief Hauteur maximale de l'aire de jeu */
#define HAUTEUR_MAX 40
/** @brief Taille d'un pavé */
#define TAILLE_PAVES 5
/** @brief Nombre de pavés */
#define NOMBRE_PAVES 4
/** @brief Nombre max de pavés autorisés */
#define MAX_PAVES 200
/** @brief Caractère pour représenter les bordures */
#define BORDURE '#'
/** @brief Caractère pour représenter le vide */
#define AIR ' '
/** @brief Zone de protection horizontale autour du serpent */
#define ZONE_DE_PROTECTION_X 15
/** @brief Zone de protection verticale autour du serpent */
#define ZONE_DE_PROTECTION_Y 5

/** @brief Position initiale X du serpent */
#define X_INITIAL 40
/** @brief Position initiale Y du serpent */
#define Y_INITIAL 20

/** @brief Caractère pour représenter la tête du serpent */
#define TETE 'O'
/** @brief Caractère pour représenter le corps du serpent */
#define CORPS 'X'
/** @brief Taille initiale du serpent */
#define TAILLE_SERPENT 10
/** @brief Vitesse de déplacement en microsecondes */
#define VITESSE_DEPLACEMENT 100000

/** @brief Touche pour aller en haut */
#define HAUT 'z'
/** @brief Touche pour aller en bas */
#define BAS 's'
/** @brief Touche pour aller à gauche */
#define GAUCHE 'q'
/** @brief Touche pour aller à droite */
#define DROITE 'd'
/** @brief Touche pour arrêter le jeu */
#define STOP 'a'

/** @typedef aireDeJeu
 * @brief Type représentant l'aire de jeu sous forme de tableau 2D.
 */
typedef char aireDeJeu[LARGEUR_MAX + 1][HAUTEUR_MAX + 1];
int pavesX[MAX_PAVES];
int pavesY[MAX_PAVES];

/**
 * @brief Affiche un caractère à une position donnée dans la console.
 *
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @param c Caractère à afficher.
 */
void afficher(int x, int y, char c);

/**
 * @brief Efface un caractère à une position donnée dans la console.
 *
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 */
void effacer(int x, int y);

/**
 * @brief Initialise l'aire de jeu avec des bordures et de l'air.
 *
 * @param tableau Tableau représentant l'aire de jeu.
 */
void initPlateau(aireDeJeu tableau);

/**
 * @brief Place un pavé dans l'aire de jeu en évitant la zone de protection.
 *
 * @param tableau Tableau représentant l'aire de jeu.
 */
void initPaves(aireDeJeu tableau);

/**
 * @brief Place un pavé dans l'aire de jeu en évitant la zone de protection.
 *
 * @param tableau Tableau représentant l'aire de jeu.
 */
bool estPositionUnique(int x, int y, int *tempX, int *tempY, int taille);

/**
 * @brief Affiche l'aire de jeu dans la console.
 *
 * @param tableau Tableau représentant l'aire de jeu.
 */
void affichagePlateau(aireDeJeu tableau);

/**
 * @brief Détermine la nouvelle direction du serpent en fonction de la touche appuyée.
 *
 * @param touche Touche saisie par l'utilisateur.
 * @param direction Direction actuelle.
 * @return Nouvelle direction.
 */
char definirDirection(char touche, char direction);

/**
 * @brief Determine l'endroit de la tête puis lui permet de changer de coté
 *
 * @param lesX Tableau des coordonnées X des segments du serpent.
 * @param lesY Tableau des coordonnées Y des segments du serpent.
 */
void teleportation(int lesX[], int lesY[]);

/**
 * @brief Dessine le serpent dans la console.
 *
 * @param lesX Tableau des coordonnées X des segments du serpent.
 * @param lesY Tableau des coordonnées Y des segments du serpent.
 */
void dessinerSerpent(int lesX[], int lesY[]);

/**
 * @brief Déplace le serpent d'une case dans la direction donnée.
 *
 * @param lesX Tableau des coordonnées X des segments du serpent.
 * @param lesY Tableau des coordonnées Y des segments du serpent.
 * @param direction Direction du mouvement ('z', 's', 'q', 'd').
 * @param statut Indique si une collision a été détectée.
 */
void progresser(int lesX[], int lesY[], char direction, bool *statut);

/**
 * @brief Affiche un message de fin de jeu et restaure les paramètres de la console.
 */
void finDuJeu();

/**
 * @brief Positionne le curseur de la console à une coordonnée donnée.
 *
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 */
void gotoXY(int x, int y);

/**
 * @brief Vérifie si une touche a été pressée sans attendre.
 *
 * @return 1 si une touche a été pressée, 0 sinon.
 */
int kbhit(void);

/**
 * @brief Désactive l'affichage des caractères tapés dans la console.
 */
void disableEcho();

/**
 * @brief Réactive l'affichage des caractères tapés dans la console.
 */
void enableEcho();

/*****************************************************
 *                 PROGRAMME PRINCIPALE              *
 *****************************************************/
int main()
{
    // Initialisation des variables .
    srand(time(NULL));
    aireDeJeu plateau;
    int x, y;
    int lesX[TAILLE_SERPENT], lesY[TAILLE_SERPENT];
    char touche = DROITE;    // Variable pour stocker la touche appuyée && mise a DROITE pour que le serpent va vers la droite
    char direction = DROITE; // Variable pour définir la direction
    initPlateau(plateau);
    system("clear");
    disableEcho();
    bool statut = false;
    // Incrémentation des coordonnées.
    x = X_INITIAL;
    y = Y_INITIAL;
    for (int i = 0; i < TAILLE_SERPENT; i++)
    {
        lesX[i] = x--;
        lesY[i] = y;
    }
    affichagePlateau(plateau);
    // déplacement du serpent tant que la touche 'a' n'a pas été enfoncer.
    do
    {
        if (kbhit())
        {
            touche = getchar(); // Lire la touche pressée
        }
        direction = definirDirection(touche, direction);
        progresser(lesX, lesY, direction, &statut);
        usleep(VITESSE_DEPLACEMENT);
    } while ((touche != STOP) && (statut != true));
    finDuJeu();
    return EXIT_SUCCESS;
}
/*****************************************************
 *                 PROCEDURE                         *
 *****************************************************/

void afficher(int x, int y, char c)
{
    /* @brief Faire l'affichage */
    if (((y >= HAUTEUR_MIN) && (y <= HAUTEUR_MAX + 1)) && ((x >= LARGEUR_MIN) && (x <= LARGEUR_MAX + 1))) // check pour savoir si la valeur a écrire se situe dans l'espace de jeu
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
void initPlateau(aireDeJeu plateau)
{
    for (int lig = 0; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 0; col <= HAUTEUR_MAX; col++)
        {
            if (((lig == LARGEUR_MIN) || (lig == LARGEUR_MAX)) || ((col == HAUTEUR_MIN) || (col == HAUTEUR_MAX)))
            {
                plateau[lig][col] = BORDURE;
            }
            else
            {
                plateau[lig][col] = AIR;
            }
        }
    }
    plateau[LARGEUR_MAX / 2][HAUTEUR_MIN] = AIR; // téléporteur du haut
    plateau[LARGEUR_MAX / 2][HAUTEUR_MAX] = AIR; // téléporteur du bas
    plateau[LARGEUR_MAX][HAUTEUR_MAX / 2] = AIR;
    plateau[LARGEUR_MIN][HAUTEUR_MAX / 2] = AIR;
    // ajout des pavés
    initPaves(plateau);
}

void initPaves(aireDeJeu plateau)
{
    int x, y;
    int coordX[MAX_PAVES];
    int coordY[MAX_PAVES];
    int compteurPaves = 0;
    bool valide = false;
    for (int i = 0; i < NOMBRE_PAVES; i++)
    {
        do
        {
            // Génération aléatoire de la position du pavé
            x = rand() % (LARGEUR_MAX - TAILLE_PAVES - 3) + 3;
            y = rand() % (HAUTEUR_MAX - TAILLE_PAVES - 3) + 3;
            coordX[compteurPaves] = x;
            coordY[compteurPaves] = y;
            compteurPaves++;

            valide = estPositionUnique(x, y, coordX, coordY, compteurPaves);

        } while ((x >= X_INITIAL - ZONE_DE_PROTECTION_X && x <= X_INITIAL + ZONE_DE_PROTECTION_X &&
                  y >= Y_INITIAL - ZONE_DE_PROTECTION_Y && y <= Y_INITIAL + ZONE_DE_PROTECTION_Y) ||
                 (valide != true));

        // Vérification de la zone de protection = la position initiale du serpent (rectangle autour du serpent(pour verifier => #define NOMBRE_PAVES 10000)).

        // AJOUT DANS LE TABLEAU
        for (int dx = 0; dx < TAILLE_PAVES; dx++)
        {
            for (int dy = 0; dy < TAILLE_PAVES; dy++)
            {
                plateau[dx + x][dy + y] = BORDURE;
                pavesX[i * TAILLE_PAVES + dx] = x + dx; // on traite les pavés avec I ,puis la taille d'un pavé X les coordonnées X
                pavesY[i * TAILLE_PAVES + dy] = y + dy; // même chose mais avec Y
            }
        }
    }
}
// Fonction pour vérifier si une paire (x, y) existe déjà
bool estPositionUnique(int x, int y, int *tempX, int *tempY, int taille)
{
    bool statut;
    for (int i = 0; i < taille; i++)
    {
        if (tempX[i] == x && tempY[i] == y)
        {
            statut = false; // La position est déjà utilisée
        }
    }
    statut = true; // La position est unique
    return statut;
}

void affichagePlateau(aireDeJeu plateau)
{
    for (int lig = 1; lig <= LARGEUR_MAX; lig++)
    {
        for (int col = 1; col <= HAUTEUR_MAX; col++)
        {
            afficher(lig, col, plateau[lig][col]);
        }
    }
}

char definirDirection(char touche, char direction)
{
    /**@brief Fonction permettant de modifier la direction de déplacement du serpent .*/

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
void teleportation(int lesX[], int lesY[])
{
    if ((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MIN))
    {
        lesX[0] = LARGEUR_MAX / 2;
        lesY[0] = HAUTEUR_MAX;
    }
    else if ((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MAX))
    {
        lesX[0] = LARGEUR_MAX / 2;
        lesY[0] = HAUTEUR_MIN;
    }
    else if ((lesX[0] == LARGEUR_MIN) && (lesY[0] == HAUTEUR_MAX / 2))
    {
        lesX[0] = LARGEUR_MAX - 1;
        lesY[0] = HAUTEUR_MAX / 2;
    }
    else if ((lesX[0] == LARGEUR_MAX - 1) && (lesY[0] == HAUTEUR_MAX / 2))
    {
        lesX[0] = LARGEUR_MIN;
        lesY[0] = HAUTEUR_MAX / 2;
    }
}

void dessinerSerpent(int lesX[], int lesY[])
{
    /** @brief On dessine le serpent */

    afficher(lesX[0], lesY[0], TETE);
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        afficher(lesX[i], lesY[i], CORPS);
    }
    fflush(stdout);
}

void progresser(int lesX[], int lesY[], char direction, bool *statut)
{
    /** @brief On efface le dernier caractère puis on déplace le serpent de 1 (modifitaction du tableau de coordonnées)*/

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
    // GESTIONS DES COLLISIONS
    // BORDURE
    if (((lesX[0] == LARGEUR_MIN) || (lesX[0] == LARGEUR_MAX - 1)) || ((lesY[0] == HAUTEUR_MIN) || (lesY[0] == HAUTEUR_MAX)))
    {
        if (((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MIN)) || ((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MAX)) || ((lesX[0] == LARGEUR_MIN) && (lesY[0] == HAUTEUR_MAX / 2)) || ((lesX[0] == LARGEUR_MAX - 1) && (lesY[0] == HAUTEUR_MAX / 2)))
        {
            *statut = false;
            teleportation(lesX, lesY);
        }
        else
        {
            *statut = true;
        }
    }

    // SERPENT
    for (int i = 1; i < TAILLE_SERPENT; i++)
    {
        if ((lesX[0] == lesX[i]) && (lesY[0] == lesY[i]))
        {
            *statut = true;
        }
    }
    // PAVES
    for (int i = 0; i < NOMBRE_PAVES; i++) // pour avoir les collisions des 4 pavés
    {
        for (int dx = 0; dx < TAILLE_PAVES; dx++) // On gère les coordonnés du pavés en X et en Y
        {
            for (int dy = 0; dy < TAILLE_PAVES; dy++)
            {
                if ((lesX[0] == pavesX[i * TAILLE_PAVES + dx]) && (lesY[0] == pavesY[i * TAILLE_PAVES + dy])) // Calcul : le pavés fois la taille + les coordonnées en X et la même chose en Y (sans le i on calcule l'es coordonnée d'un seul pavé.)
                {
                    *statut = true;
                }
            }
        }
    }

    dessinerSerpent(lesX, lesY);
}
void finDuJeu()
{
    /* @brief Fin du programme , message de fin et réactivation de l'écriture dans la console*/
    enableEcho();
    gotoXY(1, 50);

    printf("La partie est terminée !");
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