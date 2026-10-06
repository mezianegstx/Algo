/*
 * 00 - LECTURE DES ENTRÉES / AFFICHAGE
 *
 * À quoi ça sert : chaque fonction lit UN format d'entrée classique.
 * Copie celle qui correspond à l'énoncé dans ton main.
 *
 * Test : ./00_lecture_entrees <mode>   (mode = 1..7, voir le main en bas)
 *
 * Rappels scanf :
 *   %d   int            %lld  long long        %lf  double (%f pour float)
 *   %s   un mot (s'arrête à l'espace)          %c   un caractère
 *   " %c" (ESPACE devant) : saute les espaces et retours à la ligne avant
 *         de lire le caractère. Sans l'espace tu lis le '\n' précédent !
 *   scanf renvoie le nombre de valeurs lues, ou EOF (-1) en fin de fichier.
 *
 * Rappels printf :
 *   %d  %lld  %f (double ET float)  %.2f (2 décimales)  %s  %c
 *   %5d (aligné sur 5 colonnes)     %05d (complété par des zéros)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Les GROS tableaux se déclarent en global : ils sont mis à 0 d'office et
 * ne font pas exploser la pile (un `int tab[1000000]` dans main peut planter). */
#define MAXN 200005
int grand_tableau[MAXN];

#define MAXL 1005   /* ADAPTER : taille max de la grille + marge */
char grille[MAXL][MAXL];

/* ---- 1. "n" puis n valeurs ------------------------------------------- */
void lire_n_puis_valeurs(void) {
	int n;
	scanf("%d", &n);
	int tab[n];                      /* tableau de taille variable, OK si n petit */
	for (int i = 0; i < n; i++) {
		scanf("%d", &tab[i]);
	}

	long long somme = 0;             /* long long dès qu'une somme peut dépasser 2.10^9 */
	for (int i = 0; i < n; i++) somme += tab[i];
	printf("n=%d somme=%lld\n", n, somme);
}

/* ---- 2. valeurs jusqu'à une sentinelle (-1) : tableau dynamique ------- */
void lire_jusqua_sentinelle(void) {
	int capacite = 4;                /* nombre de cases allouées */
	int n = 0;                       /* nombre de cases utilisées */
	int *tab = malloc(capacite * sizeof(int));   /* NE PAS oublier sizeof(int) */

	int v;
	while (scanf("%d", &v) == 1 && v != -1) {    /* ADAPTER : la sentinelle */
		if (n == capacite) {         /* plein : on double */
			capacite *= 2;
			tab = realloc(tab, capacite * sizeof(int));
		}
		tab[n++] = v;
	}

	printf("%d valeurs lues, derniere=%d\n", n, n > 0 ? tab[n - 1] : -1);
	free(tab);
}

/* ---- 3. valeurs jusqu'à la fin du fichier (pas de n, pas de sentinelle) */
void lire_jusqua_eof(void) {
	int v, n = 0;
	while (scanf("%d", &v) == 1) {   /* s'arrête tout seul à la fin de l'entrée */
		grand_tableau[n++] = v;
	}
	printf("%d valeurs lues\n", n);
}

/* ---- 4. n mots ------------------------------------------------------- */
void lire_mots(void) {
	int n;
	scanf("%d", &n);
	char mot[101];                   /* 100 caractères max + le '\0' final */
	for (int i = 0; i < n; i++) {
		scanf("%100s", mot);         /* %100s : jamais plus de 100 caractères */
		printf("mot %d : %s (longueur %d)\n", i, mot, (int)strlen(mot));
	}
}

/* ---- 5. une lettre puis un nombre sur chaque ligne (ex : "Z 4") ------- */
void lire_lettre_nombre(void) {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++) {
		char type;
		int valeur;
		scanf(" %c %d", &type, &valeur);   /* l'ESPACE avant %c est indispensable */
		printf("type=%c valeur=%d\n", type, valeur);
	}
}

/* ---- 6. grille de caractères : "lignes colonnes" puis les lignes ------ */
void lire_grille(void) {
	int nl, nc;
	scanf("%d %d", &nl, &nc);
	for (int i = 0; i < nl; i++) {
		scanf("%s", grille[i]);      /* une ligne = un mot (pas d'espace dedans) */
	}
	/* grille[i][j] = caractère ligne i, colonne j */
	printf("coin haut-gauche=%c coin bas-droit=%c\n", grille[0][0], grille[nl - 1][nc - 1]);
}

/* ---- 6 bis. grille SANS dimensions : on lit jusqu'à la fin ------------ */
void lire_grille_sans_dimensions(void) {
	int nl = 0;
	while (scanf("%s", grille[nl]) == 1) nl++;
	int nc = (int)strlen(grille[0]);
	printf("%d lignes, %d colonnes\n", nl, nc);
}

/* ---- 7. une ligne entière, espaces compris ---------------------------- */
void lire_ligne_entiere(void) {
	char ligne[1001];
	/* fgets garde le '\n' final : on l'enlève.
	 * ATTENTION : après un scanf("%d"), il reste un '\n' dans l'entrée ;
	 * fais un premier fgets "à vide" (ou scanf(" ") ) avant de lire la ligne. */
	if (fgets(ligne, sizeof(ligne), stdin) != NULL) {
		ligne[strcspn(ligne, "\r\n")] = '\0';
		printf("ligne lue : [%s]\n", ligne);
	}
}

int main(int argc, char *argv[]) {
	int mode = argc > 1 ? atoi(argv[1]) : 1;
	switch (mode) {
		case 1: lire_n_puis_valeurs(); break;
		case 2: lire_jusqua_sentinelle(); break;
		case 3: lire_jusqua_eof(); break;
		case 4: lire_mots(); break;
		case 5: lire_lettre_nombre(); break;
		case 6: lire_grille(); break;
		case 7: lire_ligne_entiere(); break;
		case 8: lire_grille_sans_dimensions(); break;
	}
	return 0;
}
