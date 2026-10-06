/*
 * 01 - qsort ET FONCTIONS DE COMPARAISON
 *
 * qsort(tab, n, sizeof(tab[0]), cmp);     ->  trie en O(n log n)
 *        |    |   |              |
 *        |    |   |              +-- fonction de comparaison (ci-dessous)
 *        |    |   +-- taille d'UNE case
 *        |    +-- nombre de cases
 *        +-- le tableau
 *
 * La fonction cmp reçoit deux POINTEURS vers des cases du tableau et renvoie :
 *     négatif  si a doit être placé AVANT b
 *     0        si égaux
 *     positif  si a doit être placé APRÈS b
 *
 * Recette pour écrire n'importe quel cmp :
 *   1. convertir les const void* dans le bon type de pointeur
 *   2. comparer avec (x > y) - (x < y)        -> croissant
 *                     (x < y) - (x > y)        -> décroissant
 *   NE PAS écrire "return x - y" : ça déborde avec de grands nombres.
 *
 * qsort n'est PAS stable : deux éléments égaux peuvent changer d'ordre.
 * Si l'ordre d'origine compte, ajoute la position comme dernier critère.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---- int -------------------------------------------------------------- */
int cmp_int_croissant(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x > y) - (x < y);
}

int cmp_int_decroissant(const void *a, const void *b) {
	int x = *(const int *)a;
	int y = *(const int *)b;
	return (x < y) - (x > y);
}

/* ---- long long / double : même recette, on change juste le type ------- */
int cmp_long_long(const void *a, const void *b) {
	long long x = *(const long long *)a;
	long long y = *(const long long *)b;
	return (x > y) - (x < y);
}

int cmp_double(const void *a, const void *b) {
	double x = *(const double *)a;
	double y = *(const double *)b;
	return (x > y) - (x < y);
}

/* ---- caractères (trier les lettres d'un mot) -------------------------- */
int cmp_char(const void *a, const void *b) {
	char x = *(const char *)a;
	char y = *(const char *)b;
	return (x > y) - (x < y);
}

/* ---- chaînes rangées dans un tableau 2D : char mots[N][LONGUEUR] ------ */
/* Chaque case EST la chaîne, donc le pointeur reçu pointe sur ses lettres. */
int cmp_chaine_tableau2d(const void *a, const void *b) {
	return strcmp((const char *)a, (const char *)b);   /* ordre alphabétique */
}

/* ---- chaînes rangées dans un tableau de pointeurs : char *mots[N] ----- */
/* Chaque case est un char*, donc on reçoit un pointeur de pointeur. */
int cmp_chaine_pointeurs(const void *a, const void *b) {
	const char *x = *(const char *const *)a;
	const char *y = *(const char *const *)b;
	return strcmp(x, y);
}

/* ---- struct avec plusieurs critères ----------------------------------- */
typedef struct {
	char nom[32];
	int note;
	int age;
	int position;   /* rang de lecture, pour départager de façon stable */
} Eleve;

/* Tri : note décroissante, puis âge croissant, puis nom alphabétique.
 * Schéma : pour chaque critère, "si différents -> on répond tout de suite". */
int cmp_eleve(const void *a, const void *b) {
	const Eleve *x = (const Eleve *)a;
	const Eleve *y = (const Eleve *)b;

	if (x->note != y->note) return (x->note < y->note) - (x->note > y->note); /* décroissant */
	if (x->age != y->age) return (x->age > y->age) - (x->age < y->age);       /* croissant   */
	int c = strcmp(x->nom, y->nom);
	if (c != 0) return c;
	return (x->position > y->position) - (x->position < y->position);         /* stabilité   */
}

/* ---- trier des INDICES sans toucher au tableau d'origine -------------- */
/* Utile quand on veut l'ordre trié mais garder tab[] intact.
 * cmp ne peut pas recevoir de paramètre en plus : on passe par un global. */
const int *g_cles;
int cmp_indice(const void *a, const void *b) {
	int i = *(const int *)a;
	int j = *(const int *)b;
	if (g_cles[i] != g_cles[j]) return (g_cles[i] > g_cles[j]) - (g_cles[i] < g_cles[j]);
	return (i > j) - (i < j);
}

int main(void) {
	/* int croissant / décroissant */
	int tab[] = {5, 2, 9, 1, 5, 6};
	int n = sizeof(tab) / sizeof(tab[0]);

	qsort(tab, n, sizeof(int), cmp_int_croissant);
	for (int i = 0; i < n; i++) printf("%d ", tab[i]);
	printf("  (attendu 1 2 5 5 6 9)\n");

	qsort(tab, n, sizeof(int), cmp_int_decroissant);
	for (int i = 0; i < n; i++) printf("%d ", tab[i]);
	printf("  (attendu 9 6 5 5 2 1)\n");

	/* trier seulement une PARTIE : cases 1 à 3 incluses -> tab + 1, 3 cases */
	qsort(tab + 1, 3, sizeof(int), cmp_int_croissant);
	for (int i = 0; i < n; i++) printf("%d ", tab[i]);
	printf("  (attendu 9 5 5 6 2 1)\n");

	/* double */
	double reels[] = {3.5, -1.0, 2.25};
	qsort(reels, 3, sizeof(double), cmp_double);
	printf("%.2f %.2f %.2f  (attendu -1.00 2.25 3.50)\n", reels[0], reels[1], reels[2]);

	/* lettres d'un mot (pratique pour les anagrammes : 2 anagrammes triés sont égaux) */
	char mot[] = "chien";
	qsort(mot, strlen(mot), sizeof(char), cmp_char);
	printf("%s  (attendu cehin)\n", mot);

	/* chaînes, version tableau 2D */
	char mots[4][20] = {"poire", "abricot", "kiwi", "banane"};
	qsort(mots, 4, sizeof(mots[0]), cmp_chaine_tableau2d);
	for (int i = 0; i < 4; i++) printf("%s ", mots[i]);
	printf("  (attendu abricot banane kiwi poire)\n");

	/* chaînes, version tableau de pointeurs */
	char *ptrs[] = {"poire", "abricot", "kiwi"};
	qsort(ptrs, 3, sizeof(char *), cmp_chaine_pointeurs);
	printf("%s %s %s  (attendu abricot kiwi poire)\n", ptrs[0], ptrs[1], ptrs[2]);

	/* struct multi-critères */
	Eleve classe[] = {
		{"Zoe", 15, 20, 0}, {"Ali", 18, 21, 1}, {"Bob", 15, 19, 2}, {"Eve", 15, 20, 3},
	};
	qsort(classe, 4, sizeof(Eleve), cmp_eleve);
	for (int i = 0; i < 4; i++) printf("%s ", classe[i].nom);
	printf("  (attendu Ali Bob Eve Zoe)\n");

	/* tri d'indices */
	int valeurs[] = {40, 10, 30, 20};
	int ordre[] = {0, 1, 2, 3};
	g_cles = valeurs;
	qsort(ordre, 4, sizeof(int), cmp_indice);
	for (int i = 0; i < 4; i++) printf("%d ", ordre[i]);
	printf("  (attendu 1 3 2 0 : positions du plus petit au plus grand)\n");

	/* bsearch : dichotomie toute faite de la bibliothèque, sur un tableau TRIÉ
	 * avec le MÊME cmp. Renvoie un pointeur sur la case, ou NULL si absent. */
	int tries[] = {1, 4, 7, 10};
	int cle = 7;
	int *trouve = bsearch(&cle, tries, 4, sizeof(int), cmp_int_croissant);
	if (trouve != NULL) printf("7 trouve a l'indice %d  (attendu 2)\n", (int)(trouve - tries));

	return 0;
}
