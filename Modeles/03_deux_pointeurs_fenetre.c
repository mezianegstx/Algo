/*
 * 03 - DEUX POINTEURS ET FENÊTRE GLISSANTE
 *
 * Idée : au lieu de deux boucles imbriquées O(n²), on fait avancer deux
 * indices qui ne reculent jamais -> O(n).
 *
 *   - deux pointeurs "qui se rapprochent" (g au début, d à la fin) sur un
 *     tableau TRIÉ : paires de somme donnée, appariements gloutons...
 *   - fenêtre glissante [g, d] qui avance : plus long / nombre de
 *     sous-tableaux CONSÉCUTIFS vérifiant une condition.
 */
#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a, y = *(const int *)b;
	return (x > y) - (x < y);
}

/* ---- 1. deux valeurs de somme x dans un tableau TRIÉ ------------------ */
/* Renvoie 1 et remplit *i, *j si trouvé. (Ton SumofTwoValues.c)
 * Si on demande les positions d'origine : trier des struct {val, pos}. */
int paire_de_somme(const int tab[], int n, int x, int *i, int *j) {
	int g = 0, d = n - 1;
	while (g < d) {
		long long s = (long long)tab[g] + tab[d];
		if (s == x) { *i = g; *j = d; return 1; }
		if (s < x) g++;      /* somme trop petite : seul moyen de l'augmenter */
		else d--;            /* somme trop grande : seul moyen de la diminuer */
	}
	return 0;
}

/* ---- 2. appariement glouton (CSES Ferris Wheel) ----------------------- */
/* Nombre minimal de nacelles, 2 personnes max par nacelle, poids max `limite`.
 * On met le plus lourd avec le plus léger si ça rentre, sinon seul. */
int nb_nacelles(int poids[], int n, int limite) {
	qsort(poids, n, sizeof(int), cmp_int);
	int g = 0, d = n - 1, nacelles = 0;
	while (g <= d) {
		if (g < d && poids[g] + poids[d] <= limite) g++;   /* le léger monte avec */
		d--;                                               /* le lourd part dans tous les cas */
		nacelles++;
	}
	return nacelles;
}

/* ---- 3. fenêtre glissante : plus long sous-tableau de somme <= limite - */
/* Valable si les valeurs sont POSITIVES (agrandir la fenêtre augmente la somme). */
int plus_longue_fenetre(const int tab[], int n, long long limite) {
	int meilleur = 0;
	long long somme = 0;
	int g = 0;
	for (int d = 0; d < n; d++) {          /* d = bord droit, avance d'un cran à chaque tour */
		somme += tab[d];                   /* on fait ENTRER tab[d] */
		while (somme > limite) {           /* ADAPTER : condition "fenêtre invalide" */
			somme -= tab[g];               /* on fait SORTIR tab[g] */
			g++;
		}
		/* ici la fenêtre [g, d] est valide */
		if (d - g + 1 > meilleur) meilleur = d - g + 1;
	}
	return meilleur;
}

/* ---- 4. nombre de sous-tableaux de somme exactement x (valeurs > 0) --- */
int nb_fenetres_de_somme(const int tab[], int n, long long x) {
	int compte = 0, g = 0;
	long long somme = 0;
	for (int d = 0; d < n; d++) {
		somme += tab[d];
		while (somme > x) somme -= tab[g++];
		if (somme == x) compte++;
	}
	return compte;
}

/* ---- 5. fenêtre de taille FIXE k : somme maximale --------------------- */
long long max_somme_taille_k(const int tab[], int n, int k) {
	long long somme = 0;
	for (int i = 0; i < k; i++) somme += tab[i];   /* première fenêtre */
	long long meilleur = somme;
	for (int d = k; d < n; d++) {
		somme += tab[d] - tab[d - k];              /* un entre, un sort */
		if (somme > meilleur) meilleur = somme;
	}
	return meilleur;
}

/* ---- 6. fusion de deux tableaux triés --------------------------------- */
/* res doit avoir na + nb cases. Renvoie le nombre de cases écrites. */
int fusionner(const int a[], int na, const int b[], int nb, int res[]) {
	int i = 0, j = 0, k = 0;
	while (i < na && j < nb) {
		if (a[i] <= b[j]) res[k++] = a[i++];
		else res[k++] = b[j++];
	}
	while (i < na) res[k++] = a[i++];   /* on vide ce qui reste */
	while (j < nb) res[k++] = b[j++];
	return k;
}

/* ---- 7. supprimer les doublons d'un tableau TRIÉ, sur place ----------- */
/* Renvoie le nouveau nombre d'éléments (= nombre de valeurs distinctes). */
int dedoublonner(int tab[], int n) {
	if (n == 0) return 0;
	int k = 1;                           /* tab[0..k-1] = partie déjà propre */
	for (int i = 1; i < n; i++) {
		if (tab[i] != tab[k - 1]) tab[k++] = tab[i];
	}
	return k;
}

int main(void) {
	int tries[] = {1, 2, 4, 7, 11, 15};
	int i, j;
	if (paire_de_somme(tries, 6, 15, &i, &j)) {
		printf("paire : indices %d et %d (attendu 2 et 4 : 4+11)\n", i, j);
	}

	int poids[] = {7, 2, 3, 9};
	printf("nacelles=%d (attendu 3)\n", nb_nacelles(poids, 4, 10));

	int tab[] = {2, 1, 5, 1, 3, 2};
	printf("plus longue fenetre somme<=7 : %d (attendu 3)\n", plus_longue_fenetre(tab, 6, 7));
	printf("nb fenetres de somme 6 : %d (attendu 3)\n", nb_fenetres_de_somme(tab, 6, 6));
	printf("max somme taille 3 : %lld (attendu 9)\n", max_somme_taille_k(tab, 6, 3));

	int a[] = {1, 4, 9}, b[] = {2, 4, 5, 10}, res[7];
	int n = fusionner(a, 3, b, 4, res);
	for (int k = 0; k < n; k++) printf("%d ", res[k]);
	printf(" (attendu 1 2 4 4 5 9 10)\n");

	n = dedoublonner(res, n);
	printf("distincts=%d (attendu 6)\n", n);
	return 0;
}
