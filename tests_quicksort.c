// Jeu de tests pour quicksort(tab, begin, end), bornes incluses.
//
// Compilation (ASan signale tout accès hors du tableau) :
//   gcc -g -Wall -fsanitize=address -o tests_quicksort tests_quicksort.c
//
// Chaque test tourne dans un processus fils : un plantage ou une boucle
// infinie fait échouer ce test sans arrêter les suivants.

#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

// QuickSort.c contient son propre main : on le renomme pour pouvoir l'inclure.
#define main quicksort_main_ignore
#include "QuickSort.c"
#undef main

#define TIMEOUT_S 1
#define GARDE 0x5EED5EED
#define MAX_N 200

static int nb_ok = 0, nb_ko = 0;

static int cmp_int(const void *a, const void *b) {
	int x = *(const int *)a, y = *(const int *)b;
	return (x > y) - (x < y);
}

static void afficher(const char *nom, const int *t, int n) {
	printf("      %-9s {", nom);
	for (int k = 0; k < n; k++) printf("%s%d", k ? ", " : "", t[k]);
	printf("}\n");
}

// Code de sortie du fils : 0 = ok, 1 = mal trié, 2 = case hors [begin, end] modifiée,
// 3 = écriture hors du tableau.
static int executer(const int *entree, int n, int begin, int end, int *sortie) {
	int buf[MAX_N + 2];
	buf[0] = GARDE;
	buf[n + 1] = GARDE;
	memcpy(buf + 1, entree, n * sizeof(int));

	quicksort(buf + 1, begin, end);

	memcpy(sortie, buf + 1, n * sizeof(int));
	if (buf[0] != GARDE || buf[n + 1] != GARDE) return 3;

	int attendu[MAX_N];
	memcpy(attendu, entree, n * sizeof(int));
	if (end >= begin) qsort(attendu + begin, end - begin + 1, sizeof(int), cmp_int);
	for (int k = 0; k < n; k++) {
		if (sortie[k] != attendu[k]) return (k < begin || k > end) ? 2 : 1;
	}
	return 0;
}

// Renvoie 1 si le test passe. verbeux : 0 = rien, 1 = tout, 2 = seulement les échecs.
static int tester(const char *nom, const int *entree, int n, int begin, int end, int verbeux) {
	int tube[2];
	if (pipe(tube) != 0) { perror("pipe"); exit(2); }

	pid_t pid = fork();
	if (pid == 0) {
		close(tube[0]);
		alarm(TIMEOUT_S);
		int sortie[MAX_N];
		int code = executer(entree, n, begin, end, sortie);
		if (write(tube[1], sortie, n * sizeof(int)) < 0) _exit(4);
		_exit(code);
	}
	close(tube[1]);

	int obtenu[MAX_N];
	ssize_t lu = 0, r;
	while (lu < (ssize_t)(n * sizeof(int)) &&
	       (r = read(tube[0], (char *)obtenu + lu, n * sizeof(int) - lu)) > 0)
		lu += r;
	close(tube[0]);

	int statut;
	waitpid(pid, &statut, 0);

	const char *raison = NULL;
	if (WIFSIGNALED(statut)) {
		int sig = WTERMSIG(statut);
		raison = sig == SIGALRM ? "boucle infinie (timeout)"
		       : sig == SIGSEGV ? "segfault (récursion infinie ou accès hors tableau)"
		       : "plantage (voir le message d'ASan au-dessus)";
	} else if (WEXITSTATUS(statut) == 1) {
		raison = "résultat mal trié ou valeurs perdues";
	} else if (WEXITSTATUS(statut) == 2) {
		raison = "une case hors de [begin, end] a été modifiée";
	} else if (WEXITSTATUS(statut) == 3) {
		raison = "écriture hors du tableau";
	} else if (WEXITSTATUS(statut) != 0) {
		raison = "plantage (voir le message d'ASan au-dessus)";
	}

	if (raison == NULL) {
		nb_ok++;
		if (verbeux == 1) printf("  OK   %s\n", nom);
		return 1;
	}

	nb_ko++;
	if (verbeux) {
		printf("  ÉCHEC %s : %s\n", nom, raison);
		printf("      appel     quicksort(tab, %d, %d)\n", begin, end);
		afficher("entrée", entree, n);
		int attendu[MAX_N];
		memcpy(attendu, entree, n * sizeof(int));
		if (end >= begin) qsort(attendu + begin, end - begin + 1, sizeof(int), cmp_int);
		afficher("attendu", attendu, n);
		if (lu == (ssize_t)(n * sizeof(int))) afficher("obtenu", obtenu, n);
	}
	return 0;
}

#define TEST(nom, ...) do { \
	int t_[] = {__VA_ARGS__}; \
	int n_ = sizeof(t_) / sizeof(t_[0]); \
	tester(nom, t_, n_, 0, n_ - 1, 1); \
} while (0)

static void tests_fixes(void) {
	printf("== Cas particuliers\n");
	int vide[1] = {0};
	tester("tableau vide (begin=0, end=-1)", vide, 0, 0, -1, 1);
	TEST("un seul élément", 42);
	TEST("deux éléments triés", 1, 2);
	TEST("deux éléments inversés", 2, 1);
	TEST("deux éléments égaux", 7, 7);
	TEST("trois éléments", 3, 1, 2);
	TEST("exemple du cours", 3, 1, 4, 1, 5);

	printf("== Ordres particuliers\n");
	TEST("déjà trié", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
	TEST("trié à l'envers", 10, 9, 8, 7, 6, 5, 4, 3, 2, 1);
	TEST("en montagne", 1, 3, 5, 7, 9, 8, 6, 4, 2);
	TEST("en vallée", 9, 7, 5, 3, 1, 2, 4, 6, 8);
	TEST("minimum au milieu", 5, 4, 3, 0, 3, 4, 5);
	TEST("maximum au milieu", 1, 2, 3, 9, 3, 2, 1);
	TEST("alternance", 1, 9, 1, 9, 1, 9, 1, 9);

	printf("== Doublons\n");
	TEST("tous égaux", 4, 4, 4, 4, 4, 4, 4, 4);
	TEST("deux valeurs", 2, 1, 2, 1, 1, 2, 2, 1);
	TEST("beaucoup de doublons", 3, 1, 3, 3, 2, 1, 3, 2, 2, 3, 1);
	TEST("pivot en double", 5, 1, 5, 5, 9, 5, 0);

	printf("== Valeurs extrêmes\n");
	TEST("négatifs", -3, -1, -4, -1, -5, -9, -2);
	TEST("négatifs et positifs", 0, -2, 5, -7, 3, 0, -1, 8);
	TEST("INT_MIN et INT_MAX", INT_MAX, 0, INT_MIN, -1, INT_MAX, 1, INT_MIN);

	printf("== Sous-tableau (les cases hors de [begin, end] ne doivent pas bouger)\n");
	int s1[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	tester("trier [2, 6]", s1, 10, 2, 6, 1);
	tester("trier [0, 4]", s1, 10, 0, 4, 1);
	tester("trier [5, 9]", s1, 10, 5, 9, 1);
	tester("trier [3, 4] (deux cases)", s1, 10, 3, 4, 1);
	tester("trier [4, 4] (une case)", s1, 10, 4, 4, 1);
}

static void tests_aleatoires(int nb, int n_max, int val_max, const char *nom) {
	int t[MAX_N];
	int k;
	// On s'arrête au premier échec : inutile d'en voir plus, et ça évite
	// d'attendre des milliers de timeouts si le tri boucle.
	for (k = 0; k < nb; k++) {
		int n = rand() % (n_max + 1);
		for (int m = 0; m < n; m++) t[m] = rand() % (2 * val_max + 1) - val_max;
		if (!tester(nom, t, n, 0, n - 1, 2)) break;
	}
	if (k == nb) printf("  OK   %s (%d tableaux)\n", nom, nb);
}

int main(int argc, char **argv) {
	unsigned graine = argc > 1 ? (unsigned)atoi(argv[1]) : 12345;
	srand(graine);

	tests_fixes();

	printf("== Tests aléatoires (graine %u, relancer avec ./tests_quicksort <graine>)\n", graine);
	tests_aleatoires(500, 8, 3, "petits tableaux, valeurs dans [-3, 3]");
	tests_aleatoires(500, 30, 5, "tableaux moyens, beaucoup de doublons");
	tests_aleatoires(300, 50, 1000, "tableaux moyens, valeurs variées");
	tests_aleatoires(100, MAX_N, 1000000, "grands tableaux");

	printf("\nBilan : %d réussis, %d échoués\n", nb_ok, nb_ko);
	return nb_ko != 0;
}
