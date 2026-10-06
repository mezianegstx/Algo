/*
 * 08 - PROGRAMMATION DYNAMIQUE SUR UNE GRILLE
 *
 * Cadre : on part du coin haut-gauche, on va au coin bas-droit, en ne se
 * déplaçant que vers la DROITE ou vers le BAS.
 * Une case (i, j) ne peut donc venir que de (i-1, j) ou de (i, j-1) :
 *     dp[i][j] = combinaison de dp[i-1][j] et dp[i][j-1]
 *
 *   compter les chemins   -> on ADDITIONNE les deux
 *   meilleur score        -> on prend le MAX (ou le MIN) des deux + la case
 *
 * Si on peut se déplacer dans les 4 directions, ce n'est PLUS de la DP :
 * c'est un parcours en largeur, voir 11_grille_parcours.c.
 */
#include <stdio.h>

#define MAXL 1005   /* ADAPTER : taille max de la grille + marge */
#define MOD 1000000007

int chemins[MAXL][MAXL];
long long cout[MAXL][MAXL];

/* ---- 1. nombre de chemins en évitant les obstacles '*' ---------------- */
int nb_chemins(int nl, int nc, char grille[][MAXL]) {
	for (int i = 0; i < nl; i++) {
		for (int j = 0; j < nc; j++) {
			if (grille[i][j] == '*') {           /* ADAPTER : caractère obstacle */
				chemins[i][j] = 0;               /* aucun chemin ne passe ici */
			} else if (i == 0 && j == 0) {
				chemins[i][j] = 1;               /* départ */
			} else {
				int haut = i > 0 ? chemins[i - 1][j] : 0;     /* hors grille = 0 */
				int gauche = j > 0 ? chemins[i][j - 1] : 0;
				chemins[i][j] = (haut + gauche) % MOD;
			}
		}
	}
	return chemins[nl - 1][nc - 1];
}

/* ---- 2. chemin de coût minimal ---------------------------------------- */
/* ADAPTER : pour un score MAXIMAL, remplacer les deux `<` par `>`. */
long long cout_min(int nl, int nc, int grille[nl][nc]) {
	for (int i = 0; i < nl; i++) {
		for (int j = 0; j < nc; j++) {
			if (i == 0 && j == 0) {
				cout[i][j] = grille[0][0];
			} else if (i == 0) {
				cout[i][j] = cout[i][j - 1] + grille[i][j];      /* 1re ligne : on ne vient que de gauche */
			} else if (j == 0) {
				cout[i][j] = cout[i - 1][j] + grille[i][j];      /* 1re colonne : on ne vient que du haut */
			} else {
				long long meilleur = cout[i - 1][j] < cout[i][j - 1] ? cout[i - 1][j] : cout[i][j - 1];
				cout[i][j] = meilleur + grille[i][j];
			}
		}
	}
	return cout[nl - 1][nc - 1];
}

/* ---- 3. retrouver le chemin (après avoir appelé cout_min) ------------- */
/* On remonte depuis l'arrivée : à chaque case on regarde d'où on est venu.
 * Écrit les déplacements ('D' = droite, 'B' = bas) dans chemin[]. */
void reconstruire_chemin(int nl, int nc, char chemin[]) {
	int i = nl - 1, j = nc - 1;
	int k = nl + nc - 2;             /* nombre de déplacements */
	chemin[k] = '\0';
	while (i > 0 || j > 0) {
		if (i == 0) { chemin[--k] = 'D'; j--; }
		else if (j == 0) { chemin[--k] = 'B'; i--; }
		else if (cout[i - 1][j] < cout[i][j - 1]) { chemin[--k] = 'B'; i--; }
		else { chemin[--k] = 'D'; j--; }
	}
}

int main(void) {
	char g1[4][MAXL] = {
		"....",
		".*..",
		"...*",
		"*...",
	};
	printf("nb chemins=%d (attendu 3)\n", nb_chemins(4, 4, g1));

	int g2[3][3] = {
		{1, 3, 1},
		{1, 5, 1},
		{4, 2, 1},
	};
	printf("cout min=%lld (attendu 7)\n", cout_min(3, 3, g2));
	char chemin[10];
	reconstruire_chemin(3, 3, chemin);
	printf("chemin=%s (attendu DDBB)\n", chemin);
	return 0;
}
