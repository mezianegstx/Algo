/*
 * 18 - TRIS ÉCRITS À LA MAIN
 *
 * En pratique : utilise qsort (01_qsort_comparaisons.c). Ce fichier sert si
 * l'énoncé INTERDIT qsort, demande d'écrire un tri précis, ou demande
 * quelque chose que seul le tri fusion donne (compter les inversions).
 *
 *   tri par insertion   O(n²)        simple, parfait pour n <= 5000
 *   tri rapide          O(n log n)   en moyenne (ton QuickSort.c, corrigé)
 *   tri fusion          O(n log n)   garanti, stable
 */
#include <stdio.h>

void echanger(int *a, int *b) { int t = *a; *a = *b; *b = t; }

/* ---- tri par insertion ------------------------------------------------ */
/* On insère chaque valeur à sa place dans la partie gauche déjà triée,
 * comme on range des cartes dans sa main. */
void tri_insertion(int tab[], int n) {
	for (int i = 1; i < n; i++) {
		int valeur = tab[i];
		int j = i - 1;
		while (j >= 0 && tab[j] > valeur) {      /* ADAPTER : < pour un tri décroissant */
			tab[j + 1] = tab[j];                 /* on décale vers la droite */
			j--;
		}
		tab[j + 1] = valeur;
	}
}

/* ---- tri rapide (quicksort) ------------------------------------------- */
/* Trie tab[debut..fin], bornes INCLUSES. Appel : tri_rapide(tab, 0, n - 1).
 *
 * Deux détails qui évitent les bugs classiques (présents dans QuickSort.c) :
 *   - on copie la VALEUR du pivot : si on garde son indice, le pivot bouge
 *     pendant les échanges et on compare à autre chose ;
 *   - boucle `while (i <= j)` avec `if (i <= j)` avant l'échange. */
void tri_rapide(int tab[], int debut, int fin) {
	if (debut >= fin) return;
	int pivot = tab[debut + (fin - debut) / 2];
	int i = debut, j = fin;
	while (i <= j) {
		while (tab[i] < pivot) i++;              /* ADAPTER : inverser les deux < > pour décroissant */
		while (tab[j] > pivot) j--;
		if (i <= j) {
			echanger(&tab[i], &tab[j]);
			i++;
			j--;
		}
	}
	tri_rapide(tab, debut, j);
	tri_rapide(tab, i, fin);
}

/* ---- tri fusion + nombre d'inversions --------------------------------- */
/* Inversion = une paire (i < j) avec tab[i] > tab[j]. Leur nombre = le
 * nombre d'échanges de voisins nécessaires pour trier ("à quel point le
 * tableau est mélangé").
 * Trie tab[debut..fin[ (fin EXCLU) et renvoie le nombre d'inversions.
 * Appel : tri_fusion(tab, tampon, 0, n) avec tampon de n cases. */
long long tri_fusion(int tab[], int tampon[], int debut, int fin) {
	if (fin - debut <= 1) return 0;
	int milieu = debut + (fin - debut) / 2;

	long long inversions = tri_fusion(tab, tampon, debut, milieu)    /* on trie chaque moitié */
	                     + tri_fusion(tab, tampon, milieu, fin);

	int i = debut, j = milieu, k = debut;                            /* puis on fusionne */
	while (i < milieu && j < fin) {
		if (tab[i] <= tab[j]) {
			tampon[k++] = tab[i++];
		} else {
			tampon[k++] = tab[j++];
			inversions += milieu - i;        /* tab[j] passe devant tout ce qui reste à gauche */
		}
	}
	while (i < milieu) tampon[k++] = tab[i++];
	while (j < fin) tampon[k++] = tab[j++];
	for (k = debut; k < fin; k++) tab[k] = tampon[k];
	return inversions;
}

/* ---- après un tri : médiane, k-ième, moyenne tronquée ----------------- */
/* (DS2019 ex2) Sur un tableau TRIÉ :
 *   minimum = tab[0]             maximum = tab[n - 1]
 *   k-ième plus petit = tab[k - 1]
 *   médiane = tab[n / 2]  (n impair) ; moyenne de tab[n/2 - 1] et tab[n/2] (n pair) */
double mediane(const int tab_trie[], int n) {
	if (n % 2 == 1) return tab_trie[n / 2];
	return (tab_trie[n / 2 - 1] + tab_trie[n / 2]) / 2.0;       /* 2.0 : division réelle */
}

void afficher(const int tab[], int n) {
	for (int i = 0; i < n; i++) printf("%d ", tab[i]);
}

int main(void) {
	int a[] = {5, 2, 9, 1, 5, 6, 0};
	tri_insertion(a, 7);
	afficher(a, 7);
	printf(" (attendu 0 1 2 5 5 6 9)\n");

	int b[] = {5, 2, 9, 1, 5, 6, 0, 3, 3, 8};
	tri_rapide(b, 0, 9);
	afficher(b, 10);
	printf(" (attendu 0 1 2 3 3 5 5 6 8 9)\n");

	int c[] = {3, 1, 2, 5, 4}, tampon[5];
	long long inv = tri_fusion(c, tampon, 0, 5);
	afficher(c, 5);
	printf(" inversions=%lld (attendu 1 2 3 4 5, inversions=3)\n", inv);

	printf("mediane=%.1f (attendu 3.0)\n", mediane(c, 5));
	int d[] = {1, 2, 3, 4};
	printf("mediane=%.1f (attendu 2.5)\n", mediane(d, 4));
	return 0;
}
