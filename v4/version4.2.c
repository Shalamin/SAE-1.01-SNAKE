/**
 * @file snake.c
 * @brief Projet SAE1.01 - Jeu Snake en console.
 *
 * Ce programme implémente un jeu Snake en console, avec des fonctionnalités de déplacement, gestion des collisions,
 * génération de pavés et affichage dynamique.
 *
 * @author Keraudren Johan
 * @version 4.2
 * @date 23/11/2024
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
#define MAX_PAVES 10000
/** @brief Caractère pour représenter les bordures */
#define BORDURE '#'
/** @brief Caractère pour représenter le vide */
#define AIR ' '
/** @brief Zone de protection horizontale autour du serpent */
#define ZONE_DE_PROTECTION_X 15
/** @brief Zone de protection verticale autour du serpent */
#define ZONE_DE_PROTECTION_Y 5

/** @brief Caractère pour représenter la pomme */
#define POMME '6'
/** @brief Nombre de pomme dans le jeu */
#define NB_POMME 10

/** @brief Position initiale X du serpent */
#define X_INITIAL 40
/** @brief Position initiale Y du serpent */
#define Y_INITIAL 20

/** @brief Caractère pour représenter la tête du serpent */
#define TETE 'O'
/** @brief Caractère pour représenter le corps du serpent */
#define CORPS 'X'
/** @brief Taille maximum du serpent */
#define TAILLE_SERPENT_MAX 20
/** @brief Taille initiale du serpent */
#define TAILLE_SERPENT_INITIAL 10
/** @brief Vitesse de déplacement en microsecondes */
#define ACCELERATION 0.9

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

int pommeX[NB_POMME];
int pommeY[NB_POMME];
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
 * @brief  Place des pommes aléatoirement sur le plateau sans être sur le serpent si sur un pavés
 *
 *
 */
void ajouterPomme(int lesX[], int LesY[], aireDeJeu plateau, int numeroPomme, bool pomme);

void afficherPomme(aireDeJeu plateau, int numeroPomme);
bool teteTouchePomme(int lesX[], int lesY[], int pommeX[], int pommeY[], int *tailleSerpent, int indice);
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
void dessinerSerpent(int lesX[], int lesY[], int tailleSerpent);

/**
 * @brief Déplace le serpent d'une case dans la direction donnée.
 *
 * @param lesX Tableau des coordonnées X des segments du serpent.
 * @param lesY Tableau des coordonnées Y des segments du serpent.
 * @param direction Direction du mouvement ('z', 's', 'q', 'd').
 * @param statut Indique si une collision a été détectée.
 */
void progresser(int lesX[], int lesY[], char direction, bool *statut, bool *pomme);

/**
 * @brief Affiche un message de fin de jeu et restaure les paramètres de la console.
 */
void finDuJeu(int numeroPomme);

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
    float vitesseSerpent = 150000;                          //
    int lesX[TAILLE_SERPENT_MAX], lesY[TAILLE_SERPENT_MAX]; // création des tableaux des coordonnées
    char touche = DROITE;                                   // Variable pour stocker la touche appuyée && mise a DROITE pour que le serpent va vers la droite
    char direction = DROITE;                                // Variable pour définir la direction
    bool pomme = false;
    int numeroPomme = 0;
    initPlateau(plateau);
    system("clear");
    disableEcho();
    bool statut = false;
    // Incrémentation des coordonnées.
    x = X_INITIAL;
    y = Y_INITIAL;

    for (int i = 0; i < TAILLE_SERPENT_INITIAL; i++)
    {
        lesX[i] = x--;
        lesY[i] = y;
    }
    ajouterPomme(lesX, lesY, plateau, numeroPomme, pomme);
    affichagePlateau(plateau);
    // déplacement du serpent tant que la touche 'a' n'a pas été enfoncer.
    do
    {

        if (kbhit())
        {
            touche = getchar(); // Lire la touche pressée
        }
        direction = definirDirection(touche, direction);
        progresser(lesX, lesY, direction, &statut, &pomme);
        if (pomme == true)
        {
            numeroPomme++;
            ajouterPomme(lesX, lesY, plateau, numeroPomme, pomme);
            affichagePlateau(plateau);
            vitesseSerpent = vitesseSerpent * ACCELERATION;
            pomme = false;
        }
        usleep(vitesseSerpent);
    } while ((touche != STOP) && (statut != true) && (numeroPomme < NB_POMME));

    finDuJeu(numeroPomme);
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

        } while ((x >= X_INITIAL - ZONE_DE_PROTECTION_X && x <= X_INITIAL + ZONE_DE_PROTECTION_X && y >= Y_INITIAL - ZONE_DE_PROTECTION_Y && y <= Y_INITIAL + ZONE_DE_PROTECTION_Y) || (valide != true));

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
void ajouterPomme(int lesX[], int lesY[], aireDeJeu plateau, int numeroPomme, bool pomme)
{
    int x, y;
    int compteurPomme = 10;
    bool positionValide = true;
    int tailleSerpent = 10;
    do
    {
        // Génération aléatoire de la position du pavé
        x = rand() % (LARGEUR_MAX - 3) + 2; // Coordonnées dans les limites
        y = rand() % (HAUTEUR_MAX - 3) + 2;
        pommeX[numeroPomme] = x;
        pommeY[numeroPomme] = y;

        positionValide = estPositionUnique(x, y, pommeX, pommeY, compteurPomme);

        // Vérifier que la pommene tombe pas sur le serpent
        for (int i = 0; i < tailleSerpent; i++)
        {
            if (lesX[i] == x && lesY[i] == y)
            {
                positionValide = false;
            }
        }

        // Vérifier que la pomme ne tombe pas sur un pavé
        for (int i = 0; i < NOMBRE_PAVES; i++)
        {
            for (int dx = 0; dx < TAILLE_PAVES; dx++)
            {
                for (int dy = 0; dy < TAILLE_PAVES; dy++)
                {
                    if (x == pavesX[i * TAILLE_PAVES + dx] &&
                        y == pavesY[i * TAILLE_PAVES + dy])
                    {
                        positionValide = false;
                    }
                }
            }
        }
    } while (positionValide != true);

    if (numeroPomme >= NB_POMME)
    {
        numeroPomme = 0; // Réinitialise le compteur si on dépasse le nombre maximum de pommes
    }
    tailleSerpent++;
    if (numeroPomme < NB_POMME)
        afficherPomme(plateau, numeroPomme);
}
void afficherPomme(aireDeJeu plateau, int numeroPomme)
{
    // Effacer toutes les anciennes pommes
    for (int i = 0; i < NB_POMME; i++)
    {
        if (i != numeroPomme) // Ne pas effacer la pomme actuelle
        {
            plateau[pommeX[i]][pommeY[i]] = AIR;
        }
    }
    // Afficher uniquement la pomme courante
    plateau[pommeX[numeroPomme]][pommeY[numeroPomme]] = POMME;
}

bool teteTouchePomme(int lesX[], int lesY[], int pommeX[], int pommeY[], int *tailleSerpent, int indice)
{
    bool pommeToucher = false;
    // Vérifier la collision avec la pomme actuelle uniquement
    if ((lesX[0] == pommeX[indice]) && (lesY[0] == pommeY[indice]))
    {
        pommeToucher = true;
        // Augmenter la taille du serpent uniquement si une pomme est touchée
        if (*tailleSerpent < TAILLE_SERPENT_MAX)
        {
            (*tailleSerpent)++;
        }
    }
    return pommeToucher;
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
        // haut
        lesX[0] = LARGEUR_MAX / 2;
        lesY[0] = HAUTEUR_MAX;
    }
    else if ((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MAX))
    {
        // bas
        lesX[0] = LARGEUR_MAX / 2;
        lesY[0] = HAUTEUR_MIN;
    }
    else if ((lesX[0] == LARGEUR_MIN) && (lesY[0] == HAUTEUR_MAX / 2))
    {
        // gauche
        lesX[0] = LARGEUR_MAX - 1;
        lesY[0] = HAUTEUR_MAX / 2;
    }
    else if ((lesX[0] == LARGEUR_MAX) && (lesY[0] == HAUTEUR_MAX / 2))
    {
        // droite
        lesX[0] = LARGEUR_MIN;
        lesY[0] = HAUTEUR_MAX / 2;
    }
}

void dessinerSerpent(int lesX[], int lesY[], int tailleSerpent)
{
    /** @brief On dessine le serpent */

    afficher(lesX[0], lesY[0], TETE);
    for (int i = 1; i < tailleSerpent; i++)
    {
        afficher(lesX[i], lesY[i], CORPS);
    }
    fflush(stdout);
}
void progresser(int lesX[], int lesY[], char direction, bool *statut, bool *pomme)
{
    static int tailleSerpent = 10;
    static int indice = 0;
    /*!
     * mise en place d'un static pour eviter de futur soucis lors des modifications de ces variables
     * pour de futurs ajoutent. (static )
     */

    if (!*pomme)
    {
        effacer(lesX[tailleSerpent - 1], lesY[tailleSerpent - 1]);
    }

    for (int i = tailleSerpent - 1; i > 0; i--)
    {
        lesX[i] = lesX[i - 1];
        lesY[i] = lesY[i - 1];
    }

    // Mise à jour de la position de la tête
    switch (direction)
    {
    case DROITE:
        lesX[0]++;
        break;
    case GAUCHE:
        lesX[0]--;
        break;
    case BAS:
        lesY[0]++;
        break;
    default:
        lesY[0]--;
        break;
    }

    // Gestion des collisions avec les bordures et téléportation
    if (((lesX[0] == LARGEUR_MIN) || (lesX[0] == LARGEUR_MAX)) ||
        ((lesY[0] == HAUTEUR_MIN) || (lesY[0] == HAUTEUR_MAX)))
    {
        if (((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MIN)) ||
            ((lesX[0] == LARGEUR_MAX / 2) && (lesY[0] == HAUTEUR_MAX)) ||
            ((lesX[0] == LARGEUR_MIN) && (lesY[0] == HAUTEUR_MAX / 2)) ||
            ((lesX[0] == LARGEUR_MAX) && (lesY[0] == HAUTEUR_MAX / 2)))
        {
            *statut = false;
            teleportation(lesX, lesY);
        }
        else
        {
            *statut = true;
        }
    }

    // Collision avec le serpent
    for (int i = 1; i < tailleSerpent; i++)
    {
        if ((lesX[0] == lesX[i]) && (lesY[0] == lesY[i]))
        {
            *statut = true;
        }
    }

    // Collision avec les pavés
    for (int i = 0; i < NOMBRE_PAVES; i++)
    {
        for (int dx = 0; dx < TAILLE_PAVES; dx++)
        {
            for (int dy = 0; dy < TAILLE_PAVES; dy++)
            {
                if ((lesX[0] == pavesX[i * TAILLE_PAVES + dx]) &&
                    (lesY[0] == pavesY[i * TAILLE_PAVES + dy]))
                {
                    *statut = true;
                }
            }
        }
    }

    // Gestion des pommes
    *pomme = teteTouchePomme(lesX, lesY, pommeX, pommeY, &tailleSerpent, indice);
    if (*pomme)
    {
        // Effacer la pomme mangée sur le plateau
        afficher(pommeX[indice], pommeY[indice], AIR);

        indice++;
        if (indice >= NB_POMME)
        {
            indice = 0;
        }
    }

    dessinerSerpent(lesX, lesY, tailleSerpent);
}

void finDuJeu(int numeroPomme)
{
    /* @brief Fin du programme , message de fin et réactivation de l'écriture dans la console*/
    enableEcho();
    gotoXY(1, 50);

    printf("La partie est terminée !\n");
    printf("Votre score est de ; %d", numeroPomme);
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