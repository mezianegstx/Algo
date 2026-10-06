/*
 * BOITE A OUTILS 1 : LECTURE ET AFFICHAGE
 * =======================================
 * Tous les cas de lecture qu'on croise dans les énoncés domjudge.
 * Le main en bas montre l'utilisation (compiler : gcc 01_lecture_affichage.c).
 *
 * TABLEAU DES FORMATS scanf / printf
 *   type          scanf      printf
 *   int           "%d"       "%d"
 *   long long     "%lld"     "%lld"
 *   unsigned      "%u"       "%u"
 *   float         "%f"       "%f"
 *   double        "%lf" (!)  "%f" ou "%lf" ou "%g"
 *   char (1)      " %c" (*)  "%c"
 *   mot           "%s"       "%s"        (pas de & devant un tableau char)
 *   (*) l'espace avant %c saute les retours à la ligne restés dans le buffer.
 *
 * LIMITES UTILES
 *   int       : environ +/- 2,1 milliards (2^31 - 1 = 2147483647)
 *   long long : environ +/- 9,2 * 10^18
 *   12! tient dans un int, 13! non. 20! tient dans un long long, 21! non.
 *   Un produit de deux int de 100 000 = 10^10 DEBORDE -> (long long)a * b
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/* 1. Lire n puis n entiers dans un tableau alloué dynamiquement           */
/* ---------------------------------------------------------------------- */
int *lire_n_entiers(int *n)
{
    int i;
    scanf("%d", n);
    int *t = malloc((*n > 0 ? *n : 1) * sizeof(int));
    for (i = 0; i < *n; i++) scanf("%d", &t[i]);
    return t;   /* penser à free(t) */
}

/* ---------------------------------------------------------------------- */
/* 2. Lire des entiers jusqu'à une SENTINELLE (ex : -1) ou la fin          */
/*    Le -1 n'est PAS stocké. Renvoie le nombre de valeurs lues.           */
/* ---------------------------------------------------------------------- */
int lire_jusqua_sentinelle(int *t, int max, int sentinelle)
{
    int n = 0, x;
    while (n < max && scanf("%d", &x) == 1 && x != sentinelle) {
        t[n++] = x;
    }
    return n;
}

/* ---------------------------------------------------------------------- */
/* 3. Lire jusqu'à la FIN DU FICHIER (quand on ne sait pas combien il y a) */
/*    scanf renvoie le nombre de valeurs lues : 1 si OK, EOF (-1) à la fin  */
/*    Version tableau qui grandit (realloc x2).                            */
/* ---------------------------------------------------------------------- */
int *lire_tout(int *n)
{
    int capacite = 16, x;
    int *t = malloc(capacite * sizeof(int));
    *n = 0;
    while (scanf("%d", &x) == 1) {
        if (*n == capacite) {
            capacite *= 2;
            t = realloc(t, capacite * sizeof(int));
        }
        t[(*n)++] = x;
    }
    return t;
}

/* ---------------------------------------------------------------------- */
/* 4. Lire une LIGNE entière (avec des espaces) : fgets                    */
/*    fgets garde le '\n' (et parfois '\r') à la fin : on les enlève.      */
/*    ATTENTION : après un scanf("%d"), le '\n' reste dans le buffer, le   */
/*    fgets suivant lirait une ligne vide. Faire un getchar() avant, ou    */
/*    utiliser scanf(" %[^\n]", ligne).                                    */
/* ---------------------------------------------------------------------- */
int lire_ligne(char *ligne, int taille)
{
    if (fgets(ligne, taille, stdin) == NULL) return 0;
    ligne[strcspn(ligne, "\r\n")] = '\0';   /* enlève \r et \n */
    return 1;
}

/* ---------------------------------------------------------------------- */
/* 5. Afficher un réel "proprement" : 6 au lieu de 6.000000, 2.5 sinon     */
/* ---------------------------------------------------------------------- */
void afficher_reel(double x)
{
    long long entier = (long long)x;
    if (x == (double)entier) printf("%lld\r\n", entier);
    else                     printf("%.10g\r\n", x);
    /* Autres formats possibles selon l'énoncé :
       printf("%.2f\r\n", x);   -> 2 décimales : 3.14
       printf("%f\r\n", x);     -> 6 décimales : 3.140000
       printf("%g\r\n", x);     -> 6 chiffres significatifs, sans zéros */
}

/* ---------------------------------------------------------------------- */
/* 6. Afficher un tableau : une valeur par ligne / sur une ligne           */
/* ---------------------------------------------------------------------- */
void afficher_par_ligne(int *t, int n)
{
    int i;
    for (i = 0; i < n; i++) printf("%d\r\n", t[i]);
}

void afficher_sur_une_ligne(int *t, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        if (i > 0) printf(" ");    /* espace ENTRE les valeurs, pas à la fin */
        printf("%d", t[i]);
    }
    printf("\r\n");
}

/* ---------------------------------------------------------------------- */
/* 7. Commandes texte (style TD4/TD5/TD6) : boucle de lecture              */
/* ---------------------------------------------------------------------- */
void exemple_boucle_commandes(void)
{
    char cmd[64];
    int v;
    while (scanf("%63s", cmd) == 1) {
        if (strcmp(cmd, "add") == 0) {           /* commande avec argument */
            scanf("%d", &v);
            /* ... */
        } else if (strcmp(cmd, "print") == 0) {  /* commande sans argument */
            /* ... */
        } else if (strcmp(cmd, "bye") == 0) {
            break;
        }
    }
}

/* ---------------------------------------------------------------------- */
/* 8. Tableau 2D dynamique (matrice lignes x colonnes)                     */
/* ---------------------------------------------------------------------- */
int **creer_matrice(int lignes, int colonnes)
{
    int i;
    int **m = malloc(lignes * sizeof(int *));
    for (i = 0; i < lignes; i++) {
        m[i] = calloc(colonnes, sizeof(int));   /* calloc = rempli de 0 */
    }
    return m;
}

void liberer_matrice(int **m, int lignes)
{
    int i;
    for (i = 0; i < lignes; i++) free(m[i]);
    free(m);
}

/* Démo : lit des entiers jusqu'à -1 et les réaffiche */
int main(void)
{
    int t[1000];
    int n = lire_jusqua_sentinelle(t, 1000, -1);
    afficher_sur_une_ligne(t, n);
    afficher_reel(10.0 / 4);
    return 0;
}
