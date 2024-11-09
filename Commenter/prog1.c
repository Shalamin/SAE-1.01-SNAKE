#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define TMAX 20

typedef struct Elem
{
    char lettre;
    struct Elem *svt;
} element;

typedef element *chaine;

typedef char typTab[TMAX];

void init(chaine *ch);
bool estV(chaine ch);
void ajT(chaine *ch, char c);
void ajQ(chaine *ch, char c);
int nbC(chaine ch);
void aff(chaine ch);
void cat(chaine ch1, chaine ch2, chaine *ch3);
bool app(chaine ch, char c);
bool ide(chaine ch1, chaine ch2);
void inv(chaine ch1, chaine *ch2);
bool pal(chaine ch1);
bool ana(chaine ch1, chaine ch2);
void ord(chaine *ch);

int main()
{
    chaine ch1, ch2;
    init(&ch1);
    init(&ch2);

    ajQ(&ch1, 'C');
    ajQ(&ch1, 'H');
    ajQ(&ch1, 'I');
    ajQ(&ch1, 'E');
    ajQ(&ch1, 'N');
    aff(ch1);

    ajQ(&ch2, 'N');
    ajQ(&ch2, 'I');
    ajQ(&ch2, 'C');
    ajQ(&ch2, 'H');
    ajQ(&ch2, 'E');
    aff(ch2);
    ord(&ch2);
    aff(ch2);

    return EXIT_SUCCESS;
}

void init(chaine *ch)
{
    *ch = NULL;
}

bool estV(chaine ch)
{
    return ch == NULL;
}

void ajT(chaine *ch, char c)
{
    element *nouveau = (element *)malloc(sizeof(element));
    nouveau->lettre = c;
    nouveau->svt = *ch;
    *ch = nouveau;
}

void ajQ(chaine *ch, char c)
{
    element *ptCourant;
    element *nouveau = (element *)malloc(sizeof(element));
    nouveau->lettre = c;
    nouveau->svt = NULL;

    if (estV(*ch))
    {
        *ch = nouveau;
    }
    else
    {
        ptCourant = *ch;
        while (ptCourant->svt != NULL)
        {
            ptCourant = ptCourant->svt;
        }
        ptCourant->svt = nouveau;
    }
}

int nbC(chaine ch)
{
    int nb = 0;
    element *ptCourant = ch;
    while (ptCourant != NULL)
    {
        ptCourant = ptCourant->svt;
        nb++;
    }
    return nb;
}

void aff(chaine ch)
{
    if (!estV(ch))
    {
        printf("%c", ch->lettre);
        aff(ch->svt);
    }
    else
    {
        printf("\n");
    }
}

bool ide(chaine ch1, chaine ch2)
{
    element *ptCourant1 = ch1;
    element *ptCourant2 = ch2;
    bool pareil = true;

    while (pareil && ptCourant1 != NULL)
    {
        if (ptCourant2 == NULL)
        {
            pareil = false;
        }
        else if (ptCourant1->lettre != ptCourant2->lettre)
        {
            pareil = false;
        }
        else
        {
            ptCourant1 = ptCourant1->svt;
            ptCourant2 = ptCourant2->svt;
        }
    }
    return pareil && ptCourant2 == NULL;
}

void inv(chaine ch1, chaine *ch2)
{
    element *ptCourant;

    ptCourant = ch1;
    while (ptCourant != NULL)
    {
        ajT(ch2, ptCourant->lettre);
        ptCourant = ptCourant->svt;
    }
}

bool pal(chaine ch1)
{
    chaine ch;

    init(&ch);
    inv(ch1, &ch);
    return ide(ch1, ch);
}

bool app(chaine ch, char c)
{
    bool trouve = false;
    element *ptCourant = ch;

    while (!trouve && ptCourant != NULL)
    {
        if (ptCourant->lettre == c)
        {
            trouve = true;
        }
        else
        {
            ptCourant = ptCourant->svt;
        }
    }
    return trouve;
}

bool supprimer(chaine *ch, char c)
{
    bool trouve = false;
    element *ptCourant = *ch;
    element *ptPrec = NULL;

    while (!trouve && ptCourant != NULL)
    {
        if (ptCourant->lettre == c)
        {
            trouve = true;
        }
        else
        {
            ptPrec = ptCourant;
            ptCourant = ptCourant->svt;
        }
    }
    if (trouve)
    {
        if (ptPrec == NULL)
        {
            *ch = ptCourant->svt;
        }
        else
        {
            ptPrec->svt = ptCourant->svt;
        }
        free(ptCourant);
        ptCourant = NULL;
    }
    return trouve;
}

bool ana(chaine ch1, chaine ch2)
{
    bool anag = true;
    element *ptCourant = ch1;
    while (anag && ptCourant != NULL)
    {
        if (!supprimer(&ch2, ptCourant->lettre))
        {
            anag = false;
        }
        ptCourant = ptCourant->svt;
    }
    return anag && estV(ch2);
}

int LvT(chaine ch, typTab t)
{
    int i = 0;
    int N = nbC(ch);

    if (N <= TMAX)
    {
        chaine pt = ch;
        while (pt != NULL)
        {
            t[i] = pt->lettre;
            i++;
            pt = pt->svt;
        }
    }
    else
    {
        N = -1;
    }
    return N;
}

void TvL(typTab t, chaine *ch, int N)
{
    int i;

    for (i = 0; i < N; i++)
    {
        ajQ(ch, t[i]);
    }
}

void trT(typTab t, int N)
{
    int i, j;
    bool inf;
    char tmp;

    for (i = 1; i < N; i++)
    {
        j = i;
        inf = true;
        while (j > 0 && inf)
        {
            if (t[j] < t[j - 1])
            {
                tmp = t[j];
                t[j] = t[j - 1];
                t[j - 1] = tmp;
                j--;
            }
            else
            {
                inf = false;
            }
        }
    }
}

void ord(chaine *ch)
{
    typTab t;
    int N = LvT(*ch, t);
    if (N != -1)
    {
        trT(t, N);
        *ch = NULL;
        TvL(t, ch, N);
    }
}
