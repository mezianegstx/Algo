/*
 * 07 - PROGRAMMATION DYNAMIQUE SUR DES SÉQUENCES
 *
 * Méthode générale pour inventer une DP :
 *   1. définir en une phrase ce que vaut dp[i]  (ou dp[i][j])
 *   2. écrire la formule qui calcule dp[i] à partir de cases DÉJÀ calculées
 *   3. remplir les cas de base (dp[0]...)
 *   4. savoir où lire la réponse (dp[n] ? le max de tout le tableau ?)
 *
 * Contenu :
 *   mémoïsation (version récursive de la DP)
 *   Kadane        : sous-tableau CONSÉCUTIF de somme maximale       O(n)
 *   LIS           : plus longue sous-suite croissante       O(n²) / O(n log n)
 *   LCS           : plus longue sous-suite commune à deux chaînes   O(n*m)
 *   distance d'édition entre deux mots                              O(n*m)
 *   "voisins interdits" : somme max sans prendre deux cases voisines O(n)
 */
#include <stdio.h>
#include <string.h>

/* ---- 0. mémoïsation : la DP écrite en récursif ------------------------ */
/* On écrit la récursion naïve, et on range chaque résultat dans un tableau
 * pour ne jamais le recalculer. Exemple : nombre de façons de monter n
 * marches en faisant des pas de 1 ou 2 marches. */
#define MAXN 100
long long memo[MAXN];   /* 0 = pas encore calculé (ADAPTER si 0 est une vraie réponse : mettre -1) */

long long nb_escaliers(int n) {
	if (n <= 1) return 1;                  /* cas de base */
	if (memo[n] != 0) return memo[n];      /* déjà calculé : on ressort tout de suite */
	memo[n] = nb_escaliers(n - 1) + nb_escaliers(n - 2);
	return memo[n];
}

/* ---- 1. Kadane : somme maximale d'un sous-tableau consécutif ---------- */
/* dp = meilleure somme d'un sous-tableau qui SE TERMINE à la case i :
 * soit on prolonge le précédent, soit on repart de tab[i] seul. */
long long somme_max_consecutive(const int tab[], int n) {
	long long ici = tab[0];        /* meilleure somme finissant à la case courante */
	long long meilleur = tab[0];   /* meilleure somme vue jusqu'ici */
	for (int i = 1; i < n; i++) {
		if (ici < 0) ici = 0;      /* un passé négatif ne sert à rien : on repart */
		ici += tab[i];
		if (ici > meilleur) meilleur = ici;
	}
	return meilleur;               /* sous-tableau NON vide (marche même si tout est négatif) */
}

/* Même chose en retenant OÙ se trouve le sous-tableau : [*debut, *fin] */
long long somme_max_avec_bornes(const int tab[], int n, int *debut, int *fin) {
	long long ici = tab[0], meilleur = tab[0];
	int debut_ici = 0;
	*debut = 0; *fin = 0;
	for (int i = 1; i < n; i++) {
		if (ici < 0) { ici = 0; debut_ici = i; }
		ici += tab[i];
		if (ici > meilleur) { meilleur = ici; *debut = debut_ici; *fin = i; }
	}
	return meilleur;
}

/* ---- 2. LIS en O(n²) : plus longue sous-suite strictement croissante -- */
/* Sous-suite = on garde des cases dans l'ordre, pas forcément voisines.
 * dp[i] = longueur de la plus longue sous-suite croissante finissant en i. */
int lis_quadratique(const int tab[], int n) {
	int dp[n];
	int meilleur = 0;
	for (int i = 0; i < n; i++) {
		dp[i] = 1;                                   /* tab[i] tout seul */
		for (int j = 0; j < i; j++) {
			if (tab[j] < tab[i] && dp[j] + 1 > dp[i]) {   /* ADAPTER : <= pour "croissante au sens large" */
				dp[i] = dp[j] + 1;
			}
		}
		if (dp[i] > meilleur) meilleur = dp[i];
	}
	return meilleur;
}

/* ---- 3. LIS en O(n log n) : même réponse, pour n grand ---------------- */
/* fins[l] = la plus petite valeur possible en fin d'une sous-suite
 * croissante de longueur l+1. Ce tableau reste trié -> dichotomie.
 * (C'est exactement la mécanique de ton Towers.c.) */
int lis_rapide(const int tab[], int n) {
	int fins[n];
	int longueur = 0;
	for (int i = 0; i < n; i++) {
		int a = 0, b = longueur;                 /* premier indice où fins[] >= tab[i] */
		while (a < b) {
			int m = a + (b - a) / 2;
			if (fins[m] < tab[i]) a = m + 1;     /* ADAPTER : <= pour "au sens large" */
			else b = m;
		}
		fins[a] = tab[i];
		if (a == longueur) longueur++;           /* tab[i] prolonge la plus longue */
	}
	return longueur;
}

/* ---- 4. LCS : plus longue sous-suite commune -------------------------- */
/* dp[i][j] = LCS des i premières lettres de a et des j premières de b. */
int lcs(const char a[], const char b[]) {
	int la = (int)strlen(a), lb = (int)strlen(b);
	int dp[la + 1][lb + 1];
	for (int i = 0; i <= la; i++) {
		for (int j = 0; j <= lb; j++) {
			if (i == 0 || j == 0) {
				dp[i][j] = 0;                              /* un mot vide : rien en commun */
			} else if (a[i - 1] == b[j - 1]) {
				dp[i][j] = dp[i - 1][j - 1] + 1;           /* même lettre : on la garde */
			} else {
				dp[i][j] = dp[i - 1][j] > dp[i][j - 1] ? dp[i - 1][j] : dp[i][j - 1];
			}
		}
	}
	return dp[la][lb];
}

/* ---- 5. distance d'édition (Levenshtein) ------------------------------ */
/* Nombre minimal d'opérations (ajouter, supprimer, remplacer une lettre)
 * pour transformer a en b. dp[i][j] = distance entre les i premières
 * lettres de a et les j premières de b. */
int distance_edition(const char a[], const char b[]) {
	int la = (int)strlen(a), lb = (int)strlen(b);
	int dp[la + 1][lb + 1];
	for (int i = 0; i <= la; i++) {
		for (int j = 0; j <= lb; j++) {
			if (i == 0) dp[i][j] = j;                      /* tout ajouter */
			else if (j == 0) dp[i][j] = i;                 /* tout supprimer */
			else {
				int remplacer = dp[i - 1][j - 1] + (a[i - 1] != b[j - 1]);   /* +0 si même lettre */
				int supprimer = dp[i - 1][j] + 1;
				int ajouter = dp[i][j - 1] + 1;
				int m = remplacer;
				if (supprimer < m) m = supprimer;
				if (ajouter < m) m = ajouter;
				dp[i][j] = m;
			}
		}
	}
	return dp[la][lb];
}

/* ---- 6. somme max sans prendre deux cases voisines -------------------- */
/* dp[i] = meilleure somme avec les i premières cases.
 * Soit on ne prend pas la case i-1 (dp[i-1]), soit on la prend et alors on
 * saute sa voisine (dp[i-2] + tab[i-1]). */
long long somme_max_sans_voisins(const int tab[], int n) {
	long long dp[n + 2];
	dp[0] = 0;
	dp[1] = tab[0] > 0 ? tab[0] : 0;
	for (int i = 2; i <= n; i++) {
		long long sans = dp[i - 1];
		long long avec = dp[i - 2] + tab[i - 1];
		dp[i] = avec > sans ? avec : sans;
	}
	return dp[n];
}

int main(void) {
	printf("escaliers(10)=%lld (attendu 89)\n", nb_escaliers(10));

	int t1[] = {-1, 3, -2, 5, 3, -5, 2, 2};
	printf("kadane=%lld (attendu 9)\n", somme_max_consecutive(t1, 8));
	int d, f;
	long long s = somme_max_avec_bornes(t1, 8, &d, &f);
	printf("kadane bornes=%lld de %d a %d (attendu 9 de 1 a 4)\n", s, d, f);
	int negatifs[] = {-5, -2, -8};
	printf("kadane tout negatif=%lld (attendu -2)\n", somme_max_consecutive(negatifs, 3));

	int t2[] = {7, 3, 5, 3, 6, 2, 9, 8};
	printf("lis n2=%d (attendu 4)\n", lis_quadratique(t2, 8));
	printf("lis rapide=%d (attendu 4)\n", lis_rapide(t2, 8));

	printf("lcs=%d (attendu 4)\n", lcs("ABCBDAB", "BDCABA"));
	printf("edition=%d (attendu 2)\n", distance_edition("LOVE", "MOVIE"));
	printf("edition=%d (attendu 4)\n", distance_edition("chat", "chiens"));

	int t3[] = {2, 7, 9, 3, 1};
	printf("sans voisins=%lld (attendu 12)\n", somme_max_sans_voisins(t3, 5));
	return 0;
}
