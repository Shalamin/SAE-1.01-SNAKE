#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>


#define TAILLE 10 //Taille du serpent
#define VITESSE 150000 //Vitesse d'avancement du serpent
#define XDEBUT 40 //Coordonnée initiale x du serpent
#define YDEBUT 20 //Coordonnée initiale y du serpent
#define TETE 'O' //Apparence tête du serpent
#define CORPS 'X' //Apparence du corps du serpent
#define ARRET 'a' //Arrêter le mouvement
#define HAUT 'z' //diriger le serpent en haut
#define BAS 's' //diriger le serpent en bas
#define DROITE 'd' //diriger le serpent à droite
#define GAUCHE 'q' //diriger le serpent à gauche
#define XPLATEAU 80
#define YPLATEAU 40
#define BORD '#'
#define VIDE ' '

typedef char tableau [XPLATEAU+1][YPLATEAU+1];


void gotoXY(int x, int y); //la procédure positionne le curseur à un endroit précis, où x et y représentent respectivement le n° de colonne et le n° de ligne
void afficher(int x, int y, char c); //la procédure affiche le caractère c à la position (x, y), où x représente le numéro de colonne et y le numéro de ligne.
void effacer(int x, int y); //la procédure affiche un espace à la position (x, y), où x représente le numéro de colonne et y le numéro de ligne.
void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]); //la procédure affiche à l’écran un à un les éléments du serpent dont les coordonnées sont fournies dans les tableaux en paramètres.
void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction, bool *collision, tableau plateau); //la procédure calcule et affiche la prochaine position du serpent
void disableEcho(); //la procédure empêche l'affichage des saisies au clavier sur l'écran
void enableEcho(); //la procédure réactive l'affichage des saisies au clavier sur l'écran
int kbhit(); // la fonction vérifie s’il y a eu frappe d’un caractère au clavier, sans que cela bloque l’exécution
int mauvaiseSaisie(char saisie, char nvSaisie); //la fonction vérifie les saisies de l'utilisateur et empêche les mauvaises d'être prise en compte
void initPlateau(tableau plateau);
void desssinerPlateau(tableau plateau);


int main(){
        int lesX[TAILLE], lesY[TAILLE];
        tableau plateau;
        int i;
        bool collision = false;
        char nvSaisie;
        char saisie = DROITE;

        initPlateau(plateau);
        system("clear");

        for(i = 0; i < TAILLE; i++)
        {
                lesX[i]= XDEBUT-i;
                lesY[i]= YDEBUT;
        }
        desssinerPlateau(plateau);
        dessinerSerpent(lesX, lesY);
        disableEcho();
        while((saisie != ARRET) && (collision != true))
        {
                progresser(lesX, lesY, saisie, &collision, plateau);
                if (kbhit()==1)
                {
                        nvSaisie = getchar();
                        if (mauvaiseSaisie(saisie,nvSaisie) != 1)
                        {
                                saisie = nvSaisie;
                        }
                }
                usleep(VITESSE);
        }
        enableEcho();
        return EXIT_SUCCESS;
}


void gotoXY(int x, int y){
        printf("\033[%d;%df", y, x);
}


void afficher(int x, int y, char c){
        gotoXY(x, y);
        printf("%c", c);
}


void effacer(int x, int y){
        gotoXY(x, y);
        printf(" ");
}


void dessinerSerpent(int lesX[TAILLE], int lesY[TAILLE]){
        int i;
        for (i=1; i < TAILLE; i++)
        {
                if(lesX[i] > 0 && lesY[i] > 0)
                {
                        afficher(lesX[i],lesY[i],CORPS);
                }
        }
        if(lesX[0] > 0 && lesY[0] > 0){
                afficher(lesX[0],lesY[0],TETE);
        }
}


void progresser(int lesX[TAILLE], int lesY[TAILLE], char direction, bool *collision, tableau plateau){
        int i;
        effacer(lesX[TAILLE-1],lesY[TAILLE-1]);
        for (i = TAILLE - 1; i > 0; i--)
        {
                lesX[i]=  lesX[i-1];
                lesY[i]=  lesY[i-1];
        }

        switch (direction){
        case HAUT:
                lesY[0] --;
                break;
        
        case GAUCHE:
                lesX[0] --;
                break;

        case BAS:
                lesY[0] ++;
                break;

        case DROITE:
                lesX[0] ++;
                break;
        }
        for (i=1 ; i<TAILLE; i++){
                if((lesX[0]==lesX[i]) && (lesY[0]==lesY[i])){
                        *collision = true;
                }
        }

        if(plateau[lesX[0]][lesY[0]] == BORD){
                *collision = true;
        }
        
        dessinerSerpent(lesX,lesY);
}


int mauvaiseSaisie(char saisie, char nvSaisie){
        //la fonction retourne:
        //1 si la saisie est incorrecte
        //0 (x reste égal à 0) si la saisie est correcte
        int x = 0;

        if((saisie == HAUT && nvSaisie == BAS) || (saisie == BAS && nvSaisie == HAUT) || (saisie == DROITE && nvSaisie == GAUCHE) || (saisie == GAUCHE && nvSaisie == DROITE)){
                x = 1;
        }

        if(nvSaisie != HAUT && nvSaisie != GAUCHE && nvSaisie != BAS &&  nvSaisie !=DROITE && nvSaisie != ARRET){
                x = 1;
        }
        return x;
}

void initPlateau(tableau plateau)
{
        for (int lig = 0; lig <= XPLATEAU; lig++)
        {
                for (int col = 0; col <= YPLATEAU; col++)
                {
                        if (((lig == 1) || (lig == XPLATEAU)) || ((col == 1) || (col == YPLATEAU)))
                        {
                                plateau[lig][col] = BORD;
                        }
                        else
                        {
                                plateau[lig][col] = VIDE;
                        }
                }
        }
}

void desssinerPlateau(tableau plateau){
        for (int lig = 1; lig <= XPLATEAU ; lig++){
                for(int col = 1; col <= YPLATEAU; col++){                        
                        afficher(lig, col, plateau[lig][col]);                      
                }
        }
}


int kbhit(){
	// la fonction retourne :
	// 1 si un caractere est present
	// 0 si pas de caractere present
	
	int unCaractere=0;
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
 
	if(ch != EOF){
		ungetc(ch, stdin);
		unCaractere=1;
	} 
	return unCaractere;
}

void disableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Desactiver le flag ECHO
    tty.c_lflag &= ~ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}



void enableEcho() {
    struct termios tty;

    // Obtenir les attributs du terminal
    if (tcgetattr(STDIN_FILENO, &tty) == -1) {
        perror("tcgetattr");
        exit(EXIT_FAILURE);
    }

    // Reactiver le flag ECHO
    tty.c_lflag |= ECHO;

    // Appliquer les nouvelles configurations
    if (tcsetattr(STDIN_FILENO, TCSANOW, &tty) == -1) {
        perror("tcsetattr");
        exit(EXIT_FAILURE);
    }
}
