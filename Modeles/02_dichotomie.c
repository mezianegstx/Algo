/*
 * 02 - DICHOTOMIE (recherche binaire)
 *
 * Deux usages :
 *   A. chercher une valeur / une position dans un tableau TRIÉ      O(log n)
 *   B. "dichotomie sur la réponse" : on cherche le plus petit X tel que
 *      possible(X) est vrai, quand possible() passe de faux à vrai une seule
 *      fois (faux faux faux VRAI vrai vrai).
 *
 * Toutes les fonctions utilisent le même schéma, celui de ton Towers.c :
 *   intervalle [a, b[  (a inclus, b EXCLU), boucle while (a < b),
 *   milieu = a + (b - a) / 2   (et pas (a+b)/2 qui peut déborder).
 * À la sortie a == b : c'est la réponse.
 */
#include <stdio.h>

/* ---- A1. premier indice i tel que tab[i] >= x  (n si aucun) ----------- */
/* C'est LA fonction de base : les autres s'en déduisent. */
int borne_inf(const int tab[], int n, int x) {
	int a = 0, b = n;
	while (a < b) {
		int m = a + (b - a) / 2;
		if (tab[m] < x) a = m + 1;   /* tab[m] trop petit : la réponse est à droite */
		else b = m;                  /* tab[m] convient : on garde m, on cherche mieux à gauche */
	}
	return a;
}

/* ---- A2. premier indice i tel que tab[i] > x  (n si aucun) ------------ */
/* Seule différence avec borne_inf : <= au lieu de <. */
int borne_sup(const int tab[], int n, int x) {
	int a = 0, b = n;
	while (a < b) {
		int m = a + (b - a) / 2;
		if (tab[m] <= x) a = m + 1;
		else b = m;
	}
	return a;
}

/* ---- A3. indice de x, ou -1 s'il est absent --------------------------- */
int rechercher(const int tab[], int n, int x) {
	int i = borne_inf(tab, n, x);
	if (i < n && tab[i] == x) return i;
	return -1;
}

/* ---- A4. ce qu'on obtient gratuitement avec les deux bornes ----------- */
/*   nombre d'occurrences de x      : borne_sup(x) - borne_inf(x)
 *   nombre de valeurs <  x         : borne_inf(x)
 *   nombre de valeurs <= x         : borne_sup(x)
 *   nombre de valeurs dans [lo,hi] : borne_sup(hi) - borne_inf(lo)
 *   plus grande valeur <= x        : tab[borne_sup(x) - 1]  (si l'indice >= 0)
 *   plus petite valeur >= x        : tab[borne_inf(x)]      (si l'indice < n) */
int nb_occurrences(const int tab[], int n, int x) {
	return borne_sup(tab, n, x) - borne_inf(tab, n, x);
}

/* ---- B. dichotomie sur la réponse ------------------------------------- */
/* Exemple : n machines, la machine i fabrique un objet en duree[i] secondes.
 * Temps minimal pour fabriquer `objectif` objets ?
 *
 * On ne sait pas calculer le temps directement, mais on sait VÉRIFIER un
 * temps donné : en t secondes on fabrique somme(t / duree[i]) objets.
 * Et si t suffit, tout temps plus grand suffit aussi -> dichotomie sur t. */
int possible(long long t, const int duree[], int n, long long objectif) {
	long long fabriques = 0;
	for (int i = 0; i < n; i++) {
		fabriques += t / duree[i];
		if (fabriques >= objectif) return 1;   /* sortie anticipée : évite aussi le débordement */
	}
	return 0;
}

long long temps_minimal(const int duree[], int n, long long objectif) {
	/* ADAPTER les bornes : a = une valeur sûrement trop petite (ou la plus petite possible),
	 *                      b = une valeur sûrement suffisante. */
	long long a = 0;
	long long b = (long long)duree[0] * objectif;
	while (a < b) {
		long long m = a + (b - a) / 2;
		if (possible(m, duree, n, objectif)) b = m;   /* m marche : on essaie plus petit */
		else a = m + 1;                               /* m ne marche pas : il faut plus  */
	}
	return a;   /* plus petit t tel que possible(t) */
}

/* ---- B bis. version "le plus GRAND X qui marche" ---------------------- */
/* (vrai vrai vrai FAUX faux) Exemple : plus grand k tel que k*k <= n.
 * Attention au milieu arrondi vers le HAUT, sinon boucle infinie. */
long long racine_entiere(long long n) {
	long long a = 0, b = 3037000499LL;   /* racine du plus grand long long */
	while (a < b) {
		long long m = a + (b - a + 1) / 2;   /* +1 : arrondi vers le haut */
		if (m * m <= n) a = m;               /* m marche : on essaie plus grand */
		else b = m - 1;
	}
	return a;
}

/* ---- C. dichotomie sur un réel ---------------------------------------- */
/* Pas de test d'arrêt compliqué : 100 tours suffisent largement. */
double racine_reelle(double x) {
	double a = 0, b = x > 1 ? x : 1;
	for (int tour = 0; tour < 100; tour++) {
		double m = (a + b) / 2;
		if (m * m < x) a = m;
		else b = m;
	}
	return a;
}

int main(void) {
	int tab[] = {1, 3, 3, 3, 7, 9};
	int n = 6;

	printf("borne_inf(3)=%d (attendu 1)\n", borne_inf(tab, n, 3));
	printf("borne_sup(3)=%d (attendu 4)\n", borne_sup(tab, n, 3));
	printf("borne_inf(4)=%d (attendu 4)\n", borne_inf(tab, n, 4));
	printf("borne_inf(10)=%d (attendu 6 = n)\n", borne_inf(tab, n, 10));
	printf("rechercher(7)=%d (attendu 4)\n", rechercher(tab, n, 7));
	printf("rechercher(5)=%d (attendu -1)\n", rechercher(tab, n, 5));
	printf("nb_occurrences(3)=%d (attendu 3)\n", nb_occurrences(tab, n, 3));

	int duree[] = {3, 2, 5};
	printf("temps_minimal=%lld (attendu 8)\n", temps_minimal(duree, 3, 7));

	printf("racine_entiere(26)=%lld (attendu 5)\n", racine_entiere(26));
	printf("racine_reelle(2)=%.6f (attendu 1.414214)\n", racine_reelle(2));
	return 0;
}
