/*
 * 09 - BACKTRACKING (retour sur trace) : ESSAYER TOUTES LES POSSIBILITÉS
 *
 * On construit une solution choix après choix. À chaque étape :
 *     pour chaque choix possible :
 *         si le choix est autorisé :
 *             FAIRE le choix
 *             appel récursif pour l'étape suivante
 *             DÉFAIRE le choix          <- le "retour sur trace"
 *
 * Squelette à adapter :
 *
 *     void explorer(int etape) {
 *         if (etape == FIN) { traiter la solution complète; return; }
 *         for (chaque choix c) {
 *             if (!autorise(c)) continue;      // élagage : on coupe tôt
 *             faire(c);
 *             explorer(etape + 1);
 *             defaire(c);
 *         }
 *     }
 *
 * Coût : exponentiel. À utiliser quand n est PETIT :
 *     sous-ensembles 2^n   -> n <= 20 environ
 *     permutations   n!    -> n <= 10 environ
 * Si n est grand, il faut une DP ou un glouton.
 *
 * Ton SacADosRec.c est déjà un backtracking (sans le "défaire" car il n'y a
 * pas d'état à restaurer).
 */
#include <stdio.h>

#define MAXN 20

/* ---- 1. tous les SOUS-ENSEMBLES : "je prends / je ne prends pas" ------ */
int n_elements;
int elements[MAXN];
int pris[MAXN];                 /* pris[i] = 1 si l'élément i est dans le sous-ensemble courant */

void sous_ensembles(int i) {
	if (i == n_elements) {      /* on a décidé pour tout le monde : solution complète */
		printf("{ ");
		for (int k = 0; k < n_elements; k++) if (pris[k]) printf("%d ", elements[k]);
		printf("}\n");
		return;
	}
	pris[i] = 0;                /* choix 1 : sans l'élément i */
	sous_ensembles(i + 1);
	pris[i] = 1;                /* choix 2 : avec l'élément i */
	sous_ensembles(i + 1);
	pris[i] = 0;                /* on défait avant de remonter */
}

/* ---- 2. toutes les PERMUTATIONS --------------------------------------- */
int perm[MAXN];                 /* perm[0..pos-1] = début de permutation déjà construit */
int utilise[MAXN];              /* utilise[v] = 1 si v est déjà placé */
int nb_permutations;

void permutations(int pos, int n) {
	if (pos == n) {
		nb_permutations++;
		for (int k = 0; k < n; k++) printf("%d ", perm[k]);
		printf("\n");
		return;
	}
	for (int v = 0; v < n; v++) {        /* ordre croissant -> sorties dans l'ordre lexicographique */
		if (utilise[v]) continue;
		utilise[v] = 1;                  /* faire */
		perm[pos] = v;
		permutations(pos + 1, n);
		utilise[v] = 0;                  /* défaire */
	}
}

/* ---- 3. toutes les COMBINAISONS de k éléments parmi n ----------------- */
int combi[MAXN];

/* pos = combien d'éléments déjà choisis, depart = plus petit candidat autorisé
 * (on choisit toujours en ordre croissant pour ne pas générer 1,2 ET 2,1) */
void combinaisons(int pos, int depart, int n, int k) {
	if (pos == k) {
		for (int i = 0; i < k; i++) printf("%d ", combi[i]);
		printf("\n");
		return;
	}
	for (int v = depart; v < n; v++) {
		combi[pos] = v;
		combinaisons(pos + 1, v + 1, n, k);
	}
}

/* ---- 4. existe-t-il un sous-ensemble de somme donnée ? + la solution -- */
/* Version backtracking du sac à dos (utile si la capacité est énorme et
 * que la DP n'est plus possible, mais n petit). */
int objets[MAXN], n_objets;
int solution[MAXN], taille_solution;

int trouver_somme(int i, long long reste) {
	if (reste == 0) return 1;                    /* gagné */
	if (i == n_objets) return 0;                 /* plus d'objets : échec */

	if (objets[i] <= reste) {                    /* élagage : inutile d'essayer si ça dépasse */
		solution[taille_solution++] = objets[i];           /* faire */
		if (trouver_somme(i + 1, reste - objets[i])) return 1;
		taille_solution--;                                 /* défaire */
	}
	return trouver_somme(i + 1, reste);          /* essayer sans l'objet i */
}

/* ---- 5. OPTIMISATION : couper en deux tas les plus équilibrés possible - */
/* (CSES Apple Division) On explore tout en gardant le meilleur résultat. */
long long poids[MAXN];
int n_poids;
long long meilleur_ecart;

void repartir(int i, long long tas_a, long long tas_b) {
	if (i == n_poids) {
		long long ecart = tas_a > tas_b ? tas_a - tas_b : tas_b - tas_a;
		if (ecart < meilleur_ecart) meilleur_ecart = ecart;
		return;
	}
	repartir(i + 1, tas_a + poids[i], tas_b);    /* l'objet i va dans le tas A */
	repartir(i + 1, tas_a, tas_b + poids[i]);    /* ... ou dans le tas B */
	/* rien à défaire : l'état est passé en paramètre */
}

/* ---- 6. N REINES : l'exemple classique avec contraintes --------------- */
/* Placer n reines sur un échiquier n x n sans qu'elles s'attaquent.
 * On place une reine par LIGNE ; on retient les colonnes et diagonales prises. */
int colonne_prise[MAXN], diag1_prise[2 * MAXN], diag2_prise[2 * MAXN];
int nb_solutions_reines;

void placer_reines(int ligne, int n) {
	if (ligne == n) { nb_solutions_reines++; return; }
	for (int col = 0; col < n; col++) {
		int d1 = ligne + col;              /* identifiant de la diagonale "/"  */
		int d2 = ligne - col + n - 1;      /* identifiant de la diagonale "\" (décalé pour rester >= 0) */
		if (colonne_prise[col] || diag1_prise[d1] || diag2_prise[d2]) continue;

		colonne_prise[col] = diag1_prise[d1] = diag2_prise[d2] = 1;   /* faire */
		placer_reines(ligne + 1, n);
		colonne_prise[col] = diag1_prise[d1] = diag2_prise[d2] = 0;   /* défaire */
	}
}

int main(void) {
	printf("-- sous-ensembles de {1,2,3} (attendu 8 lignes)\n");
	n_elements = 3;
	elements[0] = 1; elements[1] = 2; elements[2] = 3;
	sous_ensembles(0);

	printf("-- permutations de 0,1,2\n");
	permutations(0, 3);
	printf("nb=%d (attendu 6)\n", nb_permutations);

	printf("-- 2 parmi 4 (attendu 6 lignes)\n");
	combinaisons(0, 0, 4, 2);

	int valeurs[] = {3, 34, 4, 12, 5, 2};
	n_objets = 6;
	for (int i = 0; i < 6; i++) objets[i] = valeurs[i];
	if (trouver_somme(0, 9)) {
		printf("somme 9 = ");
		for (int i = 0; i < taille_solution; i++) printf("%d ", solution[i]);
		printf(" (attendu 3 4 2)\n");
	}
	taille_solution = 0;
	printf("somme 1 possible : %d (attendu 0)\n", trouver_somme(0, 1));

	long long p[] = {3, 2, 7, 4, 1};
	n_poids = 5;
	for (int i = 0; i < 5; i++) poids[i] = p[i];
	meilleur_ecart = 1000000000000000000LL;   /* "infini" */
	repartir(0, 0, 0);
	printf("ecart minimal=%lld (attendu 1)\n", meilleur_ecart);

	placer_reines(0, 8);
	printf("8 reines : %d solutions (attendu 92)\n", nb_solutions_reines);
	return 0;
}
