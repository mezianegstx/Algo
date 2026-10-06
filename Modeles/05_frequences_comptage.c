/*
 * 05 - TABLEAUX DE FRÉQUENCES / COMPTAGE
 *
 * Idée : un tableau compte[v] = nombre de fois où v apparaît.
 * Marche quand les valeurs sont "petites" (lettres, chiffres, entiers <= 10^6).
 * Si les valeurs sont grandes (jusqu'à 10^9) : on TRIE puis on compte les
 * blocs de valeurs égales.
 *
 * PIÈGE vu dans DS2022/r2.c : un compteur de type `char` déborde à 127.
 * Toujours `int` pour compter.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a, y = *(const int *)b;
	return (x > y) - (x < y);
}

/* ---- 1. fréquence des lettres d'un mot -------------------------------- */
/* compte doit avoir 26 cases. 'a' -> 0, 'b' -> 1, ... 'z' -> 25.
 * ADAPTER : - 'A' pour des majuscules, - '0' pour des chiffres. */
void compter_lettres(const char mot[], int compte[26]) {
	for (int i = 0; i < 26; i++) compte[i] = 0;
	for (int i = 0; mot[i] != '\0'; i++) {
		compte[mot[i] - 'a']++;
	}
}

/* ---- 2. deux mots sont-ils des anagrammes ? --------------------------- */
int sont_anagrammes(const char a[], const char b[]) {
	int ca[26], cb[26];
	compter_lettres(a, ca);
	compter_lettres(b, cb);
	for (int i = 0; i < 26; i++) {
		if (ca[i] != cb[i]) return 0;
	}
	return 1;
}

/* ---- 3. peut-on réarranger le mot en palindrome ? --------------------- */
/* Oui si au plus UNE lettre apparaît un nombre impair de fois. */
int palindrome_possible(const char mot[]) {
	int compte[26];
	compter_lettres(mot, compte);
	int impairs = 0;
	for (int i = 0; i < 26; i++) {
		if (compte[i] % 2 == 1) impairs++;
	}
	return impairs <= 1;
}

/* ---- 4. valeur la plus fréquente, petites valeurs (0..MAXV) ----------- */
#define MAXV 1000000
int compte_valeurs[MAXV + 1];   /* global : déjà rempli de 0 */

int plus_frequent_petites_valeurs(const int tab[], int n) {
	for (int i = 0; i < n; i++) compte_valeurs[tab[i]]++;
	int meilleur = tab[0];
	for (int i = 0; i < n; i++) {
		int v = tab[i];
		/* ADAPTER le départage en cas d'égalité : ici on garde la plus GRANDE valeur */
		if (compte_valeurs[v] > compte_valeurs[meilleur] ||
		    (compte_valeurs[v] == compte_valeurs[meilleur] && v > meilleur)) {
			meilleur = v;
		}
	}
	for (int i = 0; i < n; i++) compte_valeurs[tab[i]] = 0;   /* on remet à zéro */
	return meilleur;
}

/* ---- 5. valeur la plus fréquente, valeurs quelconques : tri + blocs --- */
/* Schéma "parcourir les blocs de valeurs égales" : à réutiliser tel quel
 * pour compter les distincts, trouver les doublons, etc. */
int plus_frequent_par_tri(int tab[], int n) {
	qsort(tab, n, sizeof(int), cmp_int);
	int meilleur = tab[0], meilleur_nb = 0;
	int i = 0;
	while (i < n) {
		int j = i;
		while (j < n && tab[j] == tab[i]) j++;   /* tab[i..j-1] = un bloc de valeurs égales */
		int nb = j - i;
		if (nb >= meilleur_nb) {                 /* >= : en cas d'égalité, la plus grande valeur */
			meilleur_nb = nb;
			meilleur = tab[i];
		}
		i = j;                                   /* bloc suivant */
	}
	return meilleur;
}

/* ---- 6. nombre de valeurs distinctes ---------------------------------- */
int nb_distincts(int tab[], int n) {
	if (n == 0) return 0;
	qsort(tab, n, sizeof(int), cmp_int);
	int distincts = 1;
	for (int i = 1; i < n; i++) {
		if (tab[i] != tab[i - 1]) distincts++;
	}
	return distincts;
}

/* ---- 7. tri par comptage : trie en O(n + MAXV) sans comparaison ------- */
void tri_comptage(int tab[], int n) {
	for (int i = 0; i < n; i++) compte_valeurs[tab[i]]++;
	int k = 0;
	for (int v = 0; v <= MAXV; v++) {
		while (compte_valeurs[v] > 0) {
			tab[k++] = v;
			compte_valeurs[v]--;
		}
	}
}

/* ---- 8. "déjà vu ?" : première valeur qui se répète ------------------- */
int premier_doublon(const int tab[], int n) {
	static char vu[MAXV + 1];
	memset(vu, 0, sizeof(vu));
	for (int i = 0; i < n; i++) {
		if (vu[tab[i]]) return tab[i];
		vu[tab[i]] = 1;
	}
	return -1;
}

int main(void) {
	printf("anagrammes chien/niche : %d (attendu 1)\n", sont_anagrammes("chien", "niche"));
	printf("anagrammes chien/chine : %d (attendu 1)\n", sont_anagrammes("chien", "chine"));
	printf("anagrammes abc/abd : %d (attendu 0)\n", sont_anagrammes("abc", "abd"));
	printf("palindrome possible aabbc : %d (attendu 1)\n", palindrome_possible("aabbc"));
	printf("palindrome possible abc : %d (attendu 0)\n", palindrome_possible("abc"));

	int t1[] = {4, 7, 4, 9, 7, 1};
	printf("plus frequent (petites) : %d (attendu 7)\n", plus_frequent_petites_valeurs(t1, 6));
	int t2[] = {4, 7, 4, 9, 7, 1};
	printf("plus frequent (tri) : %d (attendu 7)\n", plus_frequent_par_tri(t2, 6));
	int t3[] = {2, 3, 2, 2, 3};
	printf("distincts : %d (attendu 2)\n", nb_distincts(t3, 5));

	int t4[] = {5, 0, 3, 5, 1};
	tri_comptage(t4, 5);
	for (int i = 0; i < 5; i++) printf("%d ", t4[i]);
	printf(" (attendu 0 1 3 5 5)\n");

	int t5[] = {3, 8, 1, 8, 3};
	printf("premier doublon : %d (attendu 8)\n", premier_doublon(t5, 5));
	return 0;
}
