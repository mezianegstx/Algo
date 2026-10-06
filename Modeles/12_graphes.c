/*
 * 12 - GRAPHES ET ARBRES
 *
 * Un graphe = des sommets (numérotés 0..n-1) reliés par des arêtes.
 * Énoncés typiques : villes et routes, amis, employés et chefs, tâches à
 * faire dans un certain ordre.
 *
 * Contenu :
 *   1. stocker le graphe (liste d'adjacence dans des tableaux)
 *   2. parcours en profondeur (DFS) : composantes connexes
 *   3. parcours en largeur (BFS)    : plus court chemin en nombre d'arêtes
 *   4. arbre : taille des sous-arbres (CSES Subordinates)
 *   5. tri topologique : ordonner des tâches avec des dépendances
 *   6. Dijkstra O(n²) : plus court chemin quand les arêtes ont un POIDS
 *   7. union-find : regrouper des éléments, savoir s'ils sont ensemble
 *
 * ATTENTION à la numérotation : les énoncés numérotent souvent de 1 à n.
 * Le plus simple : faire u-- et v-- juste après la lecture.
 */
#include <stdio.h>

#define MAXN 200005              /* ADAPTER : nombre max de sommets */
#define MAXM 400005              /* ADAPTER : nombre max d'arêtes x2 (non orienté = 2 arcs) */
#define INFINI 1000000000000000000LL

int n;                           /* nombre de sommets */

/* ---- 1. liste d'adjacence -------------------------------------------- */
/* Pour chaque sommet u on veut la liste de ses voisins. On chaîne les arcs :
 *   premier[u]  = numéro du premier arc qui part de u (-1 si aucun)
 *   suivant[a]  = numéro de l'arc suivant qui part du même sommet (-1 = fin)
 *   cible[a]    = sommet où mène l'arc a
 *   poids[a]    = longueur de l'arc a (si besoin)
 *
 * Pour parcourir les voisins de u :
 *   for (int a = premier[u]; a != -1; a = suivant[a]) { int v = cible[a]; ... }
 *
 * Si n est petit (<= 1000), une matrice int adj[n][n] (adj[u][v] = 1 s'il y
 * a une arête) est plus simple et marche aussi bien. */
int premier[MAXN];
int suivant[MAXM], cible[MAXM], poids[MAXM];
int nb_arcs;

void initialiser_graphe(int nb_sommets) {
	n = nb_sommets;
	nb_arcs = 0;
	for (int u = 0; u < n; u++) premier[u] = -1;
}

void ajouter_arc(int u, int v, int p) {      /* arc ORIENTÉ u -> v */
	cible[nb_arcs] = v;
	poids[nb_arcs] = p;
	suivant[nb_arcs] = premier[u];           /* on s'insère en tête de la liste de u */
	premier[u] = nb_arcs;
	nb_arcs++;
}

void ajouter_arete(int u, int v, int p) {    /* arête NON orientée = un arc dans chaque sens */
	ajouter_arc(u, v, p);
	ajouter_arc(v, u, p);
}

/* ---- 2. DFS : composantes connexes ------------------------------------ */
int composante[MAXN];            /* numéro de la composante de chaque sommet, -1 = pas vu */

void dfs(int u, int numero) {
	composante[u] = numero;
	for (int a = premier[u]; a != -1; a = suivant[a]) {
		int v = cible[a];
		if (composante[v] == -1) dfs(v, numero);
	}
}

/* Renvoie le nombre de groupes de sommets reliés entre eux.
 * u et v sont reliés  <=>  composante[u] == composante[v]. */
int compter_composantes(void) {
	for (int u = 0; u < n; u++) composante[u] = -1;
	int nb = 0;
	for (int u = 0; u < n; u++) {
		if (composante[u] == -1) {
			dfs(u, nb);
			nb++;
		}
	}
	return nb;
}

/* ---- 3. BFS : distance en nombre d'arêtes depuis un sommet ------------ */
int distance[MAXN];              /* -1 = inaccessible */
int parent[MAXN];                /* sommet par lequel on est arrivé (pour reconstruire le chemin) */
int file[MAXN];

void bfs(int depart) {
	for (int u = 0; u < n; u++) { distance[u] = -1; parent[u] = -1; }
	int tete = 0, queue = 0;
	file[queue++] = depart;
	distance[depart] = 0;
	while (tete < queue) {
		int u = file[tete++];
		for (int a = premier[u]; a != -1; a = suivant[a]) {
			int v = cible[a];
			if (distance[v] == -1) {
				distance[v] = distance[u] + 1;
				parent[v] = u;
				file[queue++] = v;
			}
		}
	}
}

/* Affiche le chemin depart -> arrivee en remontant les parents (après bfs
 * ou dijkstra). La récursion sert juste à afficher dans le bon sens. */
void afficher_chemin(int arrivee) {
	if (arrivee == -1) return;
	afficher_chemin(parent[arrivee]);
	printf("%d ", arrivee);
}

/* ---- 4. arbre : taille du sous-arbre de chaque sommet ----------------- */
/* Arbre = graphe sans cycle. Ici on a mis des arcs chef -> subordonné.
 * taille[u] = u + tous ses descendants. Le nombre de subordonnés est donc
 * taille[u] - 1. Le calcul se fait "en remontant" : d'abord les enfants. */
int taille[MAXN];

void calculer_tailles(int u) {
	taille[u] = 1;
	for (int a = premier[u]; a != -1; a = suivant[a]) {
		int v = cible[a];
		calculer_tailles(v);
		taille[u] += taille[v];
	}
}

/* ---- 5. tri topologique (algorithme de Kahn) -------------------------- */
/* Arcs u -> v = "u doit être fait AVANT v". On sort un ordre valide.
 * Principe : on peut faire une tâche dès qu'il ne lui reste plus aucun
 * prérequis. Renvoie 0 s'il y a un cycle (aucun ordre possible). */
int degre_entrant[MAXN];         /* nombre de prérequis pas encore faits */

int tri_topologique(int ordre[]) {
	for (int u = 0; u < n; u++) degre_entrant[u] = 0;
	for (int a = 0; a < nb_arcs; a++) degre_entrant[cible[a]]++;

	int tete = 0, queue = 0;
	for (int u = 0; u < n; u++) {
		if (degre_entrant[u] == 0) file[queue++] = u;    /* tâches sans prérequis */
	}
	int nb = 0;
	while (tete < queue) {
		int u = file[tete++];
		ordre[nb++] = u;
		for (int a = premier[u]; a != -1; a = suivant[a]) {
			int v = cible[a];
			degre_entrant[v]--;                           /* u est fait : un prérequis de moins pour v */
			if (degre_entrant[v] == 0) file[queue++] = v;
		}
	}
	return nb == n;              /* si on n'a pas tout sorti, il y a un cycle */
}

/* ---- 6. Dijkstra en O(n²) : plus court chemin avec des poids >= 0 ----- */
/* À chaque tour : on prend le sommet non traité le plus proche (sa distance
 * est définitive), et on améliore ses voisins. Suffisant pour n <= 5000. */
long long dist_ponderee[MAXN];
char traite[MAXN];

void dijkstra(int depart) {
	for (int u = 0; u < n; u++) { dist_ponderee[u] = INFINI; traite[u] = 0; parent[u] = -1; }
	dist_ponderee[depart] = 0;

	for (int tour = 0; tour < n; tour++) {
		int u = -1;
		for (int x = 0; x < n; x++) {                     /* le plus proche pas encore traité */
			if (!traite[x] && (u == -1 || dist_ponderee[x] < dist_ponderee[u])) u = x;
		}
		if (u == -1 || dist_ponderee[u] == INFINI) break; /* le reste est inaccessible */
		traite[u] = 1;

		for (int a = premier[u]; a != -1; a = suivant[a]) {
			int v = cible[a];
			if (dist_ponderee[u] + poids[a] < dist_ponderee[v]) {
				dist_ponderee[v] = dist_ponderee[u] + poids[a];
				parent[v] = u;
			}
		}
	}
}

/* ---- 7. union-find (ensembles disjoints) ------------------------------ */
/* Deux opérations quasi instantanées :
 *   trouver(x)  : le "représentant" du groupe de x
 *   unir(a, b)  : fusionne les groupes de a et de b
 * a et b sont dans le même groupe  <=>  trouver(a) == trouver(b).
 * Utile quand on AJOUTE des liens au fur et à mesure (pas besoin de graphe). */
int chef[MAXN];
int effectif[MAXN];              /* taille du groupe, valable pour un représentant */

void initialiser_groupes(int nb) {
	for (int i = 0; i < nb; i++) { chef[i] = i; effectif[i] = 1; }
}

int trouver(int x) {
	while (chef[x] != x) {
		chef[x] = chef[chef[x]];             /* raccourci : on se rapproche du représentant */
		x = chef[x];
	}
	return x;
}

/* Renvoie 1 si une fusion a eu lieu, 0 s'ils étaient déjà ensemble. */
int unir(int a, int b) {
	a = trouver(a);
	b = trouver(b);
	if (a == b) return 0;
	if (effectif[a] < effectif[b]) { int t = a; a = b; b = t; }   /* le petit rejoint le gros */
	chef[b] = a;
	effectif[a] += effectif[b];
	return 1;
}

int main(void) {
	/* graphe non orienté : 0-1, 1-2, 3-4, 5 isolé */
	initialiser_graphe(6);
	ajouter_arete(0, 1, 1);
	ajouter_arete(1, 2, 1);
	ajouter_arete(3, 4, 1);
	printf("composantes=%d (attendu 3)\n", compter_composantes());
	printf("0 et 2 relies : %d (attendu 1), 0 et 3 : %d (attendu 0)\n",
	       composante[0] == composante[2], composante[0] == composante[3]);

	/* BFS : 0-1, 0-2, 1-3, 2-3, 3-4 */
	initialiser_graphe(5);
	ajouter_arete(0, 1, 1);
	ajouter_arete(0, 2, 1);
	ajouter_arete(1, 3, 1);
	ajouter_arete(2, 3, 1);
	ajouter_arete(3, 4, 1);
	bfs(0);
	printf("distance 0->4 = %d (attendu 3), chemin : ", distance[4]);
	afficher_chemin(4);
	printf("\n");

	/* arbre CSES Subordinates : chefs des employés 2..5 = 1 1 2 3 */
	int chefs[] = {1, 1, 2, 3};
	initialiser_graphe(5);
	for (int employe = 1; employe < 5; employe++) {
		ajouter_arc(chefs[employe - 1] - 1, employe, 1);   /* -1 : l'énoncé numérote à partir de 1 */
	}
	calculer_tailles(0);
	printf("subordonnes : ");
	for (int u = 0; u < 5; u++) printf("%d ", taille[u] - 1);
	printf(" (attendu 4 1 1 0 0)\n");

	/* tri topologique : 0->1, 0->2, 1->3, 2->3 */
	initialiser_graphe(4);
	ajouter_arc(0, 1, 1);
	ajouter_arc(0, 2, 1);
	ajouter_arc(1, 3, 1);
	ajouter_arc(2, 3, 1);
	int ordre[4];
	if (tri_topologique(ordre)) {
		printf("ordre : %d %d %d %d (attendu 0 en premier, 3 en dernier)\n", ordre[0], ordre[1], ordre[2], ordre[3]);
	}
	ajouter_arc(3, 0, 1);        /* on crée un cycle */
	printf("avec cycle : %d (attendu 0)\n", tri_topologique(ordre));

	/* Dijkstra : 0-1 (4), 0-2 (1), 2-1 (2), 1-3 (5) */
	initialiser_graphe(4);
	ajouter_arete(0, 1, 4);
	ajouter_arete(0, 2, 1);
	ajouter_arete(2, 1, 2);
	ajouter_arete(1, 3, 5);
	dijkstra(0);
	printf("dijkstra 0->3 = %lld (attendu 8), chemin : ", dist_ponderee[3]);
	afficher_chemin(3);
	printf(" (attendu 0 2 1 3)\n");

	/* union-find */
	initialiser_groupes(5);
	unir(0, 1);
	unir(3, 4);
	printf("0~1 : %d (attendu 1), 1~3 : %d (attendu 0)\n", trouver(0) == trouver(1), trouver(1) == trouver(3));
	unir(1, 4);
	printf("apres union, 0~3 : %d (attendu 1), taille du groupe : %d (attendu 4)\n",
	       trouver(0) == trouver(3), effectif[trouver(0)]);
	return 0;
}
