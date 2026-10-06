/*
 * 04 - SOMMES PRÉFIXES (cumuls)
 *
 * Problème type : "on pose q questions du style : somme des cases l à r ?"
 * Naïf : une boucle par question -> O(n * q), trop lent.
 * Astuce : on précalcule pref[i] = somme des i PREMIÈRES cases.
 *          Alors somme(l..r) = pref[r+1] - pref[l]     -> O(1) par question.
 *
 *   tab  :      3   1   4   1   5
 *   pref :  0   3   4   8   9  14        (une case de plus, pref[0] = 0)
 *
 * Trois variantes ici : 1D, 2D (rectangles d'une grille), et l'inverse
 * ("tableau de différences") pour AJOUTER une valeur sur tout un intervalle.
 */
#include <stdio.h>

#define MAXN 200005
long long pref[MAXN];        /* long long : une somme déborde vite un int */

/* ---- 1D --------------------------------------------------------------- */
void construire_prefixes(const int tab[], int n) {
	pref[0] = 0;
	for (int i = 0; i < n; i++) {
		pref[i + 1] = pref[i] + tab[i];
	}
}

/* somme de tab[l] à tab[r], bornes INCLUSES, indices à partir de 0 */
long long somme_intervalle(int l, int r) {
	return pref[r + 1] - pref[l];
}

/* ---- 2D : somme d'un rectangle dans une grille ------------------------ */
#define MAXL 1005
long long pref2[MAXL][MAXL];   /* pref2[i][j] = somme du rectangle (0,0)..(i-1,j-1) */

void construire_prefixes_2d(int nl, int nc, int grille[nl][nc]) {
	for (int i = 0; i < nl; i++) {
		for (int j = 0; j < nc; j++) {
			/* haut + gauche - coin compté deux fois + la case elle-même */
			pref2[i + 1][j + 1] = pref2[i][j + 1] + pref2[i + 1][j] - pref2[i][j] + grille[i][j];
		}
	}
}

/* somme du rectangle de la case (l1,c1) à la case (l2,c2), coins INCLUS */
long long somme_rectangle(int l1, int c1, int l2, int c2) {
	return pref2[l2 + 1][c2 + 1] - pref2[l1][c2 + 1] - pref2[l2 + 1][c1] + pref2[l1][c1];
}

/* ---- tableau de différences : "+v sur toutes les cases de l à r" ------ */
/* On note juste +v à l'entrée et -v à la sortie ; une somme cumulée à la
 * fin redonne les vraies valeurs. Chaque mise à jour coûte O(1). */
long long diff[MAXN];

void ajouter_sur_intervalle(int l, int r, int v) {
	diff[l] += v;
	diff[r + 1] -= v;
}

void appliquer_differences(long long resultat[], int n) {
	long long courant = 0;
	for (int i = 0; i < n; i++) {
		courant += diff[i];
		resultat[i] = courant;
	}
}

int main(void) {
	int tab[] = {3, 1, 4, 1, 5};
	construire_prefixes(tab, 5);
	printf("somme(1..3)=%lld (attendu 6)\n", somme_intervalle(1, 3));
	printf("somme(0..4)=%lld (attendu 14)\n", somme_intervalle(0, 4));
	printf("somme(2..2)=%lld (attendu 4)\n", somme_intervalle(2, 2));

	/* Astuce : pour COMPTER les cases qui vérifient une propriété dans un
	 * intervalle (ex : nombre d'arbres '*'), fais les préfixes d'un tableau
	 * de 0 et de 1. */
	int grille[3][4] = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
	};
	construire_prefixes_2d(3, 4, grille);
	printf("rectangle (1,1)-(2,2)=%lld (attendu 34)\n", somme_rectangle(1, 1, 2, 2));
	printf("rectangle entier=%lld (attendu 78)\n", somme_rectangle(0, 0, 2, 3));

	long long res[6];
	ajouter_sur_intervalle(1, 3, 5);
	ajouter_sur_intervalle(2, 5, 2);
	appliquer_differences(res, 6);
	for (int i = 0; i < 6; i++) printf("%lld ", res[i]);
	printf(" (attendu 0 5 7 7 2 2)\n");
	return 0;
}
