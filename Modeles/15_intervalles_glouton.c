/*
 * 15 - INTERVALLES ET ALGORITHMES GLOUTONS
 *
 * Glouton = on TRIE les données selon le bon critère, puis on fait un seul
 * passage en prenant à chaque fois le choix "localement évident".
 * Tout l'art est dans le critère de tri :
 *
 *   fusionner des intervalles qui se chevauchent   -> trier par DÉBUT
 *   caser un maximum d'activités sans chevauchement -> trier par FIN
 *   nombre max d'intervalles ouverts en même temps  -> balayage d'événements
 *
 * Ton Intervalles2.c fait déjà la fusion avec des événements et un tri à
 * bulles ; la version "tri par début" ci-dessous est plus courte et, avec
 * qsort, passe en O(n log n).
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int debut;
	int fin;
} Intervalle;

int cmp_par_debut(const void *a, const void *b) {
	const Intervalle *x = a, *y = b;
	if (x->debut != y->debut) return (x->debut > y->debut) - (x->debut < y->debut);
	return (x->fin > y->fin) - (x->fin < y->fin);
}

int cmp_par_fin(const void *a, const void *b) {
	const Intervalle *x = a, *y = b;
	if (x->fin != y->fin) return (x->fin > y->fin) - (x->fin < y->fin);
	return (x->debut > y->debut) - (x->debut < y->debut);
}

/* ---- 0. longueur de l'intersection de deux intervalles ---------------- */
/* Formule à retenir (remplace les 4 cas de ton Intervalles.c) :
 * intersection = [max des débuts, min des fins], vide si max > min. */
int longueur_intersection(int a1, int b1, int a2, int b2) {
	int debut = a1 > a2 ? a1 : a2;
	int fin = b1 < b2 ? b1 : b2;
	return fin > debut ? fin - debut : 0;
}

/* ---- 1. fusionner les intervalles qui se chevauchent ------------------ */
/* Résultat écrit au début de t[]. Renvoie le nombre d'intervalles restants. */
int fusionner(Intervalle t[], int n) {
	if (n == 0) return 0;
	qsort(t, n, sizeof(Intervalle), cmp_par_debut);
	int k = 0;                                   /* t[k] = intervalle en cours de construction */
	for (int i = 1; i < n; i++) {
		if (t[i].debut <= t[k].fin) {            /* ADAPTER : < si deux intervalles qui se touchent restent séparés */
			if (t[i].fin > t[k].fin) t[k].fin = t[i].fin;   /* on prolonge */
		} else {
			k++;                                 /* trou : on commence un nouvel intervalle */
			t[k] = t[i];
		}
	}
	return k + 1;
}

/* ---- 2. nombre maximal d'activités compatibles (CSES Movie Festival) -- */
/* On prend toujours l'activité qui FINIT le plus tôt : elle laisse le plus
 * de place pour la suite. */
int max_activites(Intervalle t[], int n) {
	qsort(t, n, sizeof(Intervalle), cmp_par_fin);
	int compte = 0;
	int libre_a = -2147483647;                   /* instant à partir duquel on est libre */
	for (int i = 0; i < n; i++) {
		if (t[i].debut >= libre_a) {             /* ADAPTER : > si finir à 5 et commencer à 5 est interdit */
			compte++;
			libre_a = t[i].fin;
		}
	}
	return compte;
}

/* ---- 3. nombre max d'intervalles ouverts en même temps ---------------- */
/* (salles nécessaires, clients présents...) Chaque intervalle donne deux
 * événements : +1 à son début, -1 à sa fin. On les trie par position et on
 * suit le compteur. */
typedef struct {
	int position;
	int type;                                    /* +1 = ouverture, -1 = fermeture */
} Evenement;

int cmp_evenement(const void *a, const void *b) {
	const Evenement *x = a, *y = b;
	if (x->position != y->position) return (x->position > y->position) - (x->position < y->position);
	/* Même position : on traite les FERMETURES (-1) d'abord, donc [1,5] et
	 * [5,8] ne comptent pas comme simultanés.
	 * ADAPTER : inverser (y->type vs x->type) s'ils doivent compter. */
	return (x->type > y->type) - (x->type < y->type);
}

int max_simultanes(const Intervalle t[], int n) {
	Evenement ev[2 * n];
	for (int i = 0; i < n; i++) {
		ev[2 * i].position = t[i].debut;   ev[2 * i].type = +1;
		ev[2 * i + 1].position = t[i].fin; ev[2 * i + 1].type = -1;
	}
	qsort(ev, 2 * n, sizeof(Evenement), cmp_evenement);

	int ouverts = 0, meilleur = 0;
	for (int i = 0; i < 2 * n; i++) {
		ouverts += ev[i].type;
		if (ouverts > meilleur) meilleur = ouverts;
	}
	return meilleur;
}

/* ---- 4. longueur totale couverte par des intervalles ------------------ */
long long longueur_couverte(Intervalle t[], int n) {
	int k = fusionner(t, n);
	long long total = 0;
	for (int i = 0; i < k; i++) total += t[i].fin - t[i].debut;
	return total;
}

/* ---- 5. glouton simple : rendre la monnaie ---------------------------- */
/* Prendre la plus grosse pièce possible à chaque fois.
 * ATTENTION : ce n'est optimal que pour des systèmes "gentils" (1 2 5 10 20 50...).
 * Avec des pièces quelconques (1 5 7 pour faire 10) le glouton se trompe :
 * il faut la DP nb_min_pieces de 06_dp_sac_a_dos.c. */
int rendu_glouton(const int pieces_decroissantes[], int n, int montant) {
	int compte = 0;
	for (int i = 0; i < n; i++) {
		compte += montant / pieces_decroissantes[i];
		montant %= pieces_decroissantes[i];
	}
	return montant == 0 ? compte : -1;
}

int main(void) {
	printf("intersection [1,5] [3,9] = %d (attendu 2)\n", longueur_intersection(1, 5, 3, 9));
	printf("intersection [1,2] [3,9] = %d (attendu 0)\n", longueur_intersection(1, 2, 3, 9));

	Intervalle a[] = {{8, 10}, {1, 3}, {2, 6}, {15, 18}, {17, 20}};
	int k = fusionner(a, 5);
	for (int i = 0; i < k; i++) printf("[%d,%d] ", a[i].debut, a[i].fin);
	printf(" (attendu [1,6] [8,10] [15,20])\n");

	Intervalle films[] = {{3, 5}, {4, 9}, {5, 8}};
	printf("activites=%d (attendu 2)\n", max_activites(films, 3));

	Intervalle clients[] = {{5, 8}, {2, 4}, {3, 9}};
	printf("simultanes=%d (attendu 2)\n", max_simultanes(clients, 3));

	Intervalle b[] = {{1, 4}, {2, 6}, {10, 12}};
	printf("longueur couverte=%lld (attendu 7)\n", longueur_couverte(b, 3));

	int euros[] = {50, 20, 10, 5, 2, 1};
	printf("rendu 88 : %d pieces (attendu 6)\n", rendu_glouton(euros, 6, 88));
	return 0;
}
