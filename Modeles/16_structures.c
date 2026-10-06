/*
 * 16 - STRUCTURES DE DONNÉES : PILE, FILE, TABLEAU DYNAMIQUE, TAS
 *
 *   PILE (stack)  : dernier entré, premier sorti. On empile / dépile au sommet.
 *                   -> parenthèses, annuler, "plus proche plus grand à gauche"
 *   FILE (queue)  : premier entré, premier sorti.
 *                   -> parcours en largeur (voir 11 et 12), simulation de guichet
 *   TABLEAU DYNAMIQUE : tableau qui grandit tout seul (malloc / realloc)
 *                   -> quand on ne connaît pas le nombre de valeurs à l'avance
 *   TAS (heap)    : donne le MINIMUM (ou maximum) en O(log n), même si on
 *                   ajoute des valeurs entre-temps.
 *                   -> "toujours traiter le plus petit / le plus urgent"
 *
 * En examen, la version "tableau global + un indice" suffit presque toujours
 * et s'écrit en trois lignes.
 */
#include <stdio.h>
#include <stdlib.h>

#define MAXN 200005

/* ---- PILE dans un tableau --------------------------------------------- */
int pile[MAXN];
int sommet = 0;                  /* nombre d'éléments ; le dessus est pile[sommet - 1] */

void empiler(int v) { pile[sommet++] = v; }
int depiler(void) { return pile[--sommet]; }
int dessus(void) { return pile[sommet - 1]; }
int pile_vide(void) { return sommet == 0; }

/* Application 1 : parenthèses bien formées ? "([]{})" oui, "(]" non */
int bien_parenthese(const char s[]) {
	sommet = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		char c = s[i];
		if (c == '(' || c == '[' || c == '{') {
			empiler(c);                                  /* un ouvrant attend son fermant */
		} else {
			if (pile_vide()) return 0;                   /* fermant sans ouvrant */
			char ouvrant = (char)depiler();
			if ((c == ')' && ouvrant != '(') || (c == ']' && ouvrant != '[') || (c == '}' && ouvrant != '{')) {
				return 0;                                /* mauvais type */
			}
		}
	}
	return pile_vide();                                  /* il ne doit rester aucun ouvrant */
}

/* Application 2 : pour chaque case, position de la plus proche valeur
 * strictement plus petite à sa gauche (-1 si aucune). O(n) grâce à la pile :
 * on y garde les indices des candidats, valeurs croissantes. */
void plus_petit_a_gauche(const int tab[], int n, int resultat[]) {
	sommet = 0;
	for (int i = 0; i < n; i++) {
		while (!pile_vide() && tab[dessus()] >= tab[i]) depiler();   /* ces candidats ne serviront plus */
		resultat[i] = pile_vide() ? -1 : dessus();
		empiler(i);
	}
}

/* ---- FILE dans un tableau --------------------------------------------- */
/* Version simple : on n'efface jamais, `tete` avance. Suffit quand chaque
 * élément entre au plus une fois (BFS). */
int file[MAXN];
int tete = 0, queue = 0;

void enfiler(int v) { file[queue++] = v; }
int defiler(void) { return file[tete++]; }
int file_vide(void) { return tete == queue; }

/* Version circulaire : si on enfile / défile sans arrêt, on réutilise la
 * place avec un modulo. */
#define TAILLE_FILE 8
int fc[TAILLE_FILE];
int fc_tete = 0, fc_nb = 0;

void fc_enfiler(int v) { fc[(fc_tete + fc_nb) % TAILLE_FILE] = v; fc_nb++; }
int fc_defiler(void) { int v = fc[fc_tete]; fc_tete = (fc_tete + 1) % TAILLE_FILE; fc_nb--; return v; }

/* ---- TABLEAU DYNAMIQUE ------------------------------------------------ */
typedef struct {
	int *donnees;
	int nb;                      /* cases utilisées */
	int capacite;                /* cases allouées */
} Tableau;

void tableau_init(Tableau *t) {
	t->nb = 0;
	t->capacite = 4;
	t->donnees = malloc(t->capacite * sizeof(int));      /* sizeof(int) : PAS malloc(4) tout court */
}

void tableau_ajouter(Tableau *t, int v) {
	if (t->nb == t->capacite) {
		t->capacite *= 2;
		t->donnees = realloc(t->donnees, t->capacite * sizeof(int));
	}
	t->donnees[t->nb++] = v;
}

/* ---- TAS MIN ---------------------------------------------------------- */
/* Arbre binaire rangé dans un tableau : les enfants de la case i sont
 * 2i+1 et 2i+2, son parent est (i-1)/2. Règle : un parent est toujours <=
 * à ses enfants, donc le minimum est en tas[0].
 * ADAPTER en tas MAX (comme ton tasbinaire.c) : inverser les deux
 * comparaisons marquées. Pour un tas de struct : changer le type et comparer
 * le champ voulu. */
int tas[MAXN];
int taille_tas = 0;

void echanger(int *a, int *b) { int t = *a; *a = *b; *b = t; }

void tas_inserer(int v) {
	int i = taille_tas++;
	tas[i] = v;
	while (i > 0 && tas[(i - 1) / 2] > tas[i]) {         /* (comparaison 1) le parent est plus grand : on monte */
		echanger(&tas[(i - 1) / 2], &tas[i]);
		i = (i - 1) / 2;
	}
}

int tas_extraire_min(void) {
	int minimum = tas[0];
	tas[0] = tas[--taille_tas];                          /* le dernier vient à la racine... */
	int i = 0;
	while (1) {                                          /* ...et redescend à sa place */
		int gauche = 2 * i + 1, droite = 2 * i + 2, plus_petit = i;
		if (gauche < taille_tas && tas[gauche] < tas[plus_petit]) plus_petit = gauche;    /* (comparaison 2) */
		if (droite < taille_tas && tas[droite] < tas[plus_petit]) plus_petit = droite;    /* (comparaison 2) */
		if (plus_petit == i) break;
		echanger(&tas[i], &tas[plus_petit]);
		i = plus_petit;
	}
	return minimum;
}

/* Application : coût minimal pour souder des barres. Souder deux barres
 * coûte la somme de leurs longueurs ; on soude toujours les deux plus
 * petites (glouton), d'où le tas. */
long long cout_soudure(const int longueurs[], int n) {
	taille_tas = 0;
	for (int i = 0; i < n; i++) tas_inserer(longueurs[i]);
	long long total = 0;
	while (taille_tas > 1) {
		int a = tas_extraire_min();
		int b = tas_extraire_min();
		total += a + b;
		tas_inserer(a + b);
	}
	return total;
}

int main(void) {
	printf("([]{}) : %d (attendu 1)\n", bien_parenthese("([]{})"));
	printf("(] : %d (attendu 0)\n", bien_parenthese("(]"));
	printf("(( : %d (attendu 0)\n", bien_parenthese("(("));

	int tab[] = {2, 5, 1, 4, 8, 3}, res[6];
	plus_petit_a_gauche(tab, 6, res);
	for (int i = 0; i < 6; i++) printf("%d ", res[i]);
	printf(" (attendu -1 0 -1 2 3 2)\n");

	enfiler(10); enfiler(20); enfiler(30);
	int premier_sorti = defiler();
	int deuxieme_sorti = defiler();
	printf("file : %d %d vide=%d (attendu 10 20 vide=0)\n", premier_sorti, deuxieme_sorti, file_vide());

	for (int tour = 0; tour < 20; tour++) { fc_enfiler(tour); fc_enfiler(tour + 100); fc_defiler(); fc_defiler(); }
	fc_enfiler(7);
	printf("file circulaire : %d (attendu 7)\n", fc_defiler());

	Tableau t;
	tableau_init(&t);
	for (int i = 0; i < 100; i++) tableau_ajouter(&t, i * i);
	printf("tableau dynamique : nb=%d dernier=%d (attendu 100 et 9801)\n", t.nb, t.donnees[t.nb - 1]);
	free(t.donnees);

	int valeurs[] = {5, 3, 8, 1, 9, 2};
	for (int i = 0; i < 6; i++) tas_inserer(valeurs[i]);
	printf("tas : ");
	while (taille_tas > 0) printf("%d ", tas_extraire_min());
	printf(" (attendu 1 2 3 5 8 9)\n");

	int barres[] = {4, 3, 2, 6};
	printf("cout soudure=%lld (attendu 29)\n", cout_soudure(barres, 4));
	return 0;
}
