//
// Created by KERAUDREN johan on 25/10/2024.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

const int HAUTEUR = 40;
const int LARGEUR = 40;
const int TSERP = 5;
const int Y0 = 1;
const int X0 = 1;
int lesX[TSERP];
int lesY[TSERP];


void gotoXY(int x, int y) {
    printf("\033[%d;%df", y, x);
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
void ligneTirets(int HAUTEUR,int x0, int y0) {
    char c = '-';
        for (x0 = 0; x0 < LARGEUR; x0++) {
          gotoXY(x0,y0);
            printf("%c", c);
        }

    }
    void ligneCourante(int HAUTEUR, int LARGEUR, int x0, int y0) {
        char c = '|';
        y0++;
        gotoXY(x0,y0);
        printf("%c", c);
        for (int x0 ; x0 < LARGEUR-2; x0++) {
            gotoXY(x0,y0);
            printf(" ");
        }
        gotoXY(x0+1,y0);
        printf("%c",c);
    }
    void corps(int HAUTEUR, int LARGEUR) {
      int x0=1;
      int y0=1;
        ligneTirets(LARGEUR,x0,y0);
        for (int i = 0; i < HAUTEUR; i++) {
            ligneCourante(HAUTEUR,LARGEUR,x0,y0);
        }
        y0 = HAUTEUR;
        ligneTirets(LARGEUR ,x0,y0);
        gotoXY(1000,10000);
    }
    void affichage(int lesX[],int lesY[],char c){
        gotoXY(lesX[0],lesY[0]);
        printf("%c",c);

       /* for (int i = 1; i < TSERP; i++) {
          gotoXY(lesX[i],lesY[i]);
          printf("x");
      }*/
     }
     void effacer(int lesX[],int lesY[]){
       for (int i = 0; i < TSERP; i++) {
         gotoXY(lesX[i],lesY[i]);
         printf(" ");
     }
     }
int main(){
    int x,y;
    int valTable = 0;
    int lesX[TSERP];
    int lesY[TSERP];
    printf("Position du snake \nx:");
    scanf("%d",&x);
    printf("y: ");
    scanf("%d",&y);
    system("clear");
    corps(HAUTEUR,LARGEUR);
    usleep(10000);
    for (int i = 0; i < TSERP ; i++){
           lesX[valTable] = x;
           valTable++;
           x--;
    }
        affichage(lesX,lesY,'O');

  }