/*
 * 10 - MASQUES DE BITS
 *
 * Un entier vu comme un ensemble : le bit numéro i vaut 1 si l'élément i
 * est dans l'ensemble. Tu l'as déjà fait dans DS2019/ex3.c (lettres vues).
 *
 *   1 << i              masque qui contient seulement l'élément i  (= 2^i)
 *   m | (1 << i)        ajouter i
 *   m & ~(1 << i)       enlever i
 *   m ^ (1 << i)        inverser i
 *   (m >> i) & 1        i est-il dedans ?   (ou : m & (1 << i))
 *   a | b               union            a & b      intersection
 *   (1 << n) - 1        ensemble plein de n éléments
 *
 * PIÈGES :
 *   - priorités : & | ^ << passent APRÈS == et +. Mets TOUJOURS des parenthèses :
 *       if ((m & (1 << i)) != 0)     et pas   if (m & 1 << i != 0)
 *   - un int a 32 bits : pour i >= 31 écrire 1LL << i avec un long long.
 *
 * Deuxième usage : énumérer tous les sous-ensembles de n éléments avec une
 * simple boucle de 0 à 2^n - 1 (au lieu d'un backtracking récursif).
 */
#include <stdio.h>

/* nombre de bits à 1 (= taille de l'ensemble) */
int nb_bits(unsigned int m) {
	int compte = 0;
	while (m != 0) {
		compte += m & 1;
		m >>= 1;
	}
	return compte;
}

/* ---- 1. ensemble de lettres : y a-t-il une lettre répétée ? ----------- */
int a_une_lettre_repetee(const char mot[]) {
	int vues = 0;
	for (int i = 0; mot[i] != '\0'; i++) {
		int bit = 1 << (mot[i] - 'a');     /* ADAPTER : - 'A' pour des majuscules */
		if ((vues & bit) != 0) return 1;
		vues |= bit;
	}
	return 0;
}

/* ---- 2. énumérer tous les sous-ensembles avec un masque --------------- */
/* Exemple : meilleur écart entre deux tas (même problème que dans
 * 09_backtracking.c). masque = les objets qui vont dans le tas A. */
long long ecart_minimal(const long long poids[], int n) {
	long long total = 0;
	for (int i = 0; i < n; i++) total += poids[i];

	long long meilleur = total;
	for (int masque = 0; masque < (1 << n); masque++) {   /* 2^n sous-ensembles */
		long long tas_a = 0;
		for (int i = 0; i < n; i++) {
			if ((masque >> i) & 1) tas_a += poids[i];      /* l'objet i est dans le masque */
		}
		long long tas_b = total - tas_a;
		long long ecart = tas_a > tas_b ? tas_a - tas_b : tas_b - tas_a;
		if (ecart < meilleur) meilleur = ecart;
	}
	return meilleur;
}

/* ---- 3. afficher un nombre en binaire --------------------------------- */
void afficher_binaire(unsigned int v, int nb_chiffres) {
	for (int i = nb_chiffres - 1; i >= 0; i--) {
		printf("%d", (v >> i) & 1);
	}
	printf("\n");
}

int main(void) {
	int m = 0;
	m |= 1 << 3;                 /* ajoute 3 */
	m |= 1 << 0;                 /* ajoute 0 */
	printf("contient 3 : %d (attendu 1)\n", (m >> 3) & 1);
	printf("contient 2 : %d (attendu 0)\n", (m >> 2) & 1);
	m &= ~(1 << 3);              /* enlève 3 */
	printf("apres retrait, m=%d (attendu 1)\n", m);

	printf("nb_bits(13)=%d (attendu 3)\n", nb_bits(13));
	printf("pair ? 6 -> %d (attendu 1)\n", (6 & 1) == 0);
	printf("puissance de 2 ? 64 -> %d (attendu 1)\n", (64 & (64 - 1)) == 0);

	printf("lettre repetee 'chien' : %d (attendu 0)\n", a_une_lettre_repetee("chien"));
	printf("lettre repetee 'lettre' : %d (attendu 1)\n", a_une_lettre_repetee("lettre"));

	long long p[] = {3, 2, 7, 4, 1};
	printf("ecart minimal=%lld (attendu 1)\n", ecart_minimal(p, 5));

	afficher_binaire(13, 8);     /* attendu 00001101 */
	return 0;
}
