/*
 * 06 - PROGRAMMATION DYNAMIQUE : FAMILLE "SAC À DOS / PIÈCES"
 *
 * C'est le thème tombé aux DS 2022 (chèvres) et 2025 (p4). Tout repose sur
 * un tableau dp[c] indexé par la CAPACITÉ (ou la somme) c, de 0 à cap.
 *
 * Il n'y a que TROIS décisions à prendre pour adapter :
 *
 * 1) Que contient dp[c] ?
 *      1/0        -> "peut-on atteindre exactement c ?"
 *      un nombre  -> "combien de façons d'atteindre c ?"
 *      un min     -> "nombre minimal d'objets pour atteindre c"
 *      un max     -> "valeur maximale avec un poids <= c"
 *
 * 2) Chaque objet est-il utilisable UNE fois ou À VOLONTÉ ?
 *      une fois  : boucle objets DEHORS, capacité DEDANS en DESCENDANT (c = cap -> poids)
 *      à volonté : capacité en MONTANT
 *    Pourquoi descendre : dp[c - poids] n'a pas encore été modifié par
 *    l'objet courant, donc on ne peut pas le prendre deux fois.
 *
 * 3) (seulement quand on COMPTE des façons, objets à volonté)
 *    L'ordre compte-t-il ?  2+3 et 3+2 = deux façons ou une seule ?
 *      ordre compte        : capacité DEHORS, objets dedans
 *      ordre ne compte pas : objets DEHORS, capacité dedans
 *
 * Complexité : O(nombre d'objets * cap) dans tous les cas.
 */
#include <stdio.h>

#define MOD 1000000007
#define INFINI 1000000000

/* ---- 1. atteignable, chaque objet UNE fois (tes SacADosOpti / chèvres) - */
/* À la fin, atteignable[c] == 1 si un sous-ensemble d'objets pèse exactement c. */
void atteignable_une_fois(const int poids[], int n, int cap, char atteignable[]) {
	for (int c = 0; c <= cap; c++) atteignable[c] = 0;
	atteignable[0] = 1;                            /* ne rien prendre = somme 0 */

	for (int o = 0; o < n; o++) {
		for (int c = cap; c >= poids[o]; c--) {    /* DESCENDANT : une seule fois */
			if (atteignable[c - poids[o]]) atteignable[c] = 1;
		}
	}
}

/* ---- 2. atteignable, objets À VOLONTÉ (ton SacADosIt, DS2025 p4) ------- */
void atteignable_a_volonte(const int poids[], int n, int cap, char atteignable[]) {
	for (int c = 0; c <= cap; c++) atteignable[c] = 0;
	atteignable[0] = 1;

	for (int c = 1; c <= cap; c++) {
		for (int o = 0; o < n; o++) {              /* o < n, pas <= n ! */
			if (poids[o] <= c && atteignable[c - poids[o]]) {
				atteignable[c] = 1;
				break;
			}
		}
	}
}

/* ---- 3. valeur maximale, chaque objet UNE fois (sac à dos 0/1) --------- */
/* Chaque objet a un poids ET une valeur. dp[c] = meilleure valeur totale
 * avec un poids total <= c. (CSES Book Shop : poids = prix, valeur = pages) */
int valeur_max_une_fois(const int poids[], const int valeur[], int n, int cap) {
	int dp[cap + 1];
	for (int c = 0; c <= cap; c++) dp[c] = 0;

	for (int o = 0; o < n; o++) {
		for (int c = cap; c >= poids[o]; c--) {
			int avec = dp[c - poids[o]] + valeur[o];   /* on prend l'objet o */
			if (avec > dp[c]) dp[c] = avec;            /* sinon dp[c] reste "sans l'objet" */
		}
	}
	return dp[cap];
}

/* ---- 4. valeur maximale, objets À VOLONTÉ ------------------------------ */
int valeur_max_a_volonte(const int poids[], const int valeur[], int n, int cap) {
	int dp[cap + 1];
	for (int c = 0; c <= cap; c++) dp[c] = 0;

	for (int o = 0; o < n; o++) {
		for (int c = poids[o]; c <= cap; c++) {        /* MONTANT : réutilisable */
			int avec = dp[c - poids[o]] + valeur[o];
			if (avec > dp[c]) dp[c] = avec;
		}
	}
	return dp[cap];
}

/* ---- 5. nombre de façons, l'ORDRE COMPTE (dés, Coin Combinations I) ---- */
/* 2+3 et 3+2 sont deux façons différentes. Résultat modulo MOD. */
int nb_facons_ordre_compte(const int pieces[], int n, int cible) {
	int dp[cible + 1];
	for (int c = 0; c <= cible; c++) dp[c] = 0;
	dp[0] = 1;                                     /* une façon de faire 0 : ne rien prendre */

	for (int c = 1; c <= cible; c++) {             /* capacité DEHORS */
		for (int p = 0; p < n; p++) {
			if (pieces[p] <= c) {
				dp[c] = (dp[c] + dp[c - pieces[p]]) % MOD;   /* le modulo à CHAQUE addition */
			}
		}
	}
	return dp[cible];
}

/* ---- 6. nombre de façons, l'ordre NE compte PAS (Coin Combinations II) - */
int nb_facons_sans_ordre(const int pieces[], int n, int cible) {
	int dp[cible + 1];
	for (int c = 0; c <= cible; c++) dp[c] = 0;
	dp[0] = 1;

	for (int p = 0; p < n; p++) {                  /* pièces DEHORS */
		for (int c = pieces[p]; c <= cible; c++) {
			dp[c] = (dp[c] + dp[c - pieces[p]]) % MOD;
		}
	}
	return dp[cible];
}

/* ---- 7. nombre MINIMAL de pièces pour faire la cible (-1 si impossible) */
int nb_min_pieces(const int pieces[], int n, int cible) {
	int dp[cible + 1];
	dp[0] = 0;
	for (int c = 1; c <= cible; c++) dp[c] = INFINI;   /* INFINI = "pas encore atteignable" */

	for (int c = 1; c <= cible; c++) {
		for (int p = 0; p < n; p++) {
			if (pieces[p] <= c && dp[c - pieces[p]] + 1 < dp[c]) {
				dp[c] = dp[c - pieces[p]] + 1;
			}
		}
	}
	return dp[cible] >= INFINI ? -1 : dp[cible];
}

/* ---- 8. sac à dos 0/1 AVEC la liste des objets choisis ----------------- */
/* Pour retrouver QUELS objets on a pris, il faut garder tout le tableau 2D :
 * dp[i][c] = meilleure valeur avec les i premiers objets et une capacité c.
 * Puis on remonte depuis dp[n][cap].
 * Remplit choisis[] (indices des objets) et renvoie leur nombre. */
int sac_a_dos_avec_choix(const int poids[], const int valeur[], int n, int cap,
                         int choisis[], int *valeur_totale) {
	static int dp[105][10005];                     /* ADAPTER : [n max + 1][cap max + 1] */

	for (int c = 0; c <= cap; c++) dp[0][c] = 0;   /* 0 objet : valeur 0 */
	for (int i = 1; i <= n; i++) {
		for (int c = 0; c <= cap; c++) {
			dp[i][c] = dp[i - 1][c];               /* on ne prend pas l'objet i-1 */
			if (poids[i - 1] <= c) {
				int avec = dp[i - 1][c - poids[i - 1]] + valeur[i - 1];
				if (avec > dp[i][c]) dp[i][c] = avec;
			}
		}
	}
	*valeur_totale = dp[n][cap];

	/* remontée : si la valeur change entre i-1 et i, c'est que l'objet i-1 a été pris */
	int nb = 0, c = cap;
	for (int i = n; i >= 1; i--) {
		if (dp[i][c] != dp[i - 1][c]) {
			choisis[nb++] = i - 1;
			c -= poids[i - 1];
		}
	}
	return nb;
}

int main(void) {
	int objets[] = {4, 2, 5, 2};
	char att[14];
	atteignable_une_fois(objets, 4, 13, att);
	printf("sommes atteignables : ");
	for (int c = 1; c <= 13; c++) if (att[c]) printf("%d ", c);
	printf(" (attendu 2 4 5 6 7 8 9 11 13)\n");

	/* variante chèvres : "la plus grande charge <= cap" = dernier c atteignable */
	atteignable_une_fois(objets, 4, 10, att);
	int plus_grand = 10;
	while (!att[plus_grand]) plus_grand--;
	printf("charge max <= 10 : %d (attendu 9)\n", plus_grand);

	int p2[] = {4, 7};
	atteignable_a_volonte(p2, 2, 13, att);
	printf("13 avec 4 et 7 a volonte : %d (attendu 0), 15 : ", att[13]);
	char att2[16];
	atteignable_a_volonte(p2, 2, 15, att2);
	printf("%d (attendu 1)\n", att2[15]);

	int prix[] = {4, 8, 5, 3}, pages[] = {5, 12, 8, 1};
	printf("valeur max une fois : %d (attendu 13)\n", valeur_max_une_fois(prix, pages, 4, 10));
	printf("valeur max a volonte : %d (attendu 16)\n", valeur_max_a_volonte(prix, pages, 4, 10));

	int pieces[] = {2, 3, 5};
	printf("facons ordre compte : %d (attendu 8)\n", nb_facons_ordre_compte(pieces, 3, 9));
	printf("facons sans ordre : %d (attendu 3)\n", nb_facons_sans_ordre(pieces, 3, 9));

	int de[] = {1, 2, 3, 4, 5, 6};
	printf("des, somme 3 : %d (attendu 4)\n", nb_facons_ordre_compte(de, 6, 3));

	int pieces2[] = {1, 5, 7};
	printf("min pieces pour 11 : %d (attendu 3)\n", nb_min_pieces(pieces2, 3, 11));
	int pieces3[] = {5, 7};
	printf("min pieces pour 3 : %d (attendu -1)\n", nb_min_pieces(pieces3, 2, 3));

	int choisis[4], total;
	int nb = sac_a_dos_avec_choix(prix, pages, 4, 10, choisis, &total);
	printf("objets choisis (valeur %d) : ", total);
	for (int i = 0; i < nb; i++) printf("%d ", choisis[i]);
	printf(" (attendu valeur 13, objets 2 0)\n");
	return 0;
}
