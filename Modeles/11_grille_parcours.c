/*
 * 11 - PARCOURS DE GRILLE : VOISINS, ALIGNEMENTS, ZONES, PLUS COURT CHEMIN
 *
 * Trois outils :
 *   1. les tableaux de directions dl[] / dc[] pour visiter les voisins
 *      sans écrire quatre fois le même code
 *   2. le REMPLISSAGE (flood fill) : marquer toute une zone connectée
 *        -> compter les zones / pièces / îles, mesurer leur taille
 *   3. le PARCOURS EN LARGEUR (BFS) : plus court chemin en nombre de pas
 *      quand tous les déplacements coûtent 1
 *
 * Complexité : O(lignes * colonnes), chaque case est visitée une fois.
 *
 * Test : ./11_grille_parcours           (exemples intégrés)
 *        ./11_grille_parcours p4 < fichier   (lit une grille, cherche 4 alignés)
 */
#include <stdio.h>
#include <string.h>

#define MAXL 1005                /* ADAPTER : taille max + marge */

int nl, nc;                      /* nombre de lignes, de colonnes */
char grille[MAXL][MAXL];
int vu[MAXL][MAXL];              /* vu[i][j] = 1 si la case a déjà été visitée */

/* Les 4 voisins : haut, bas, gauche, droite.
 * ADAPTER : pour 8 voisins (avec les diagonales), utiliser
 *   dl8[] = {-1,-1,-1, 0, 0, 1, 1, 1}   dc8[] = {-1, 0, 1,-1, 1,-1, 0, 1} */
const int dl[4] = {-1, 1, 0, 0};
const int dc[4] = {0, 0, -1, 1};
const char nom_direction[4] = {'H', 'B', 'G', 'D'};

int dans_grille(int l, int c) {
	return l >= 0 && l < nl && c >= 0 && c < nc;
}

/* ---- 1. remplissage récursif (DFS) ------------------------------------ */
/* Marque toute la zone libre qui contient (l, c). Renvoie sa taille.
 * ATTENTION : sur une très grande grille (1000 x 1000 toute libre) la
 * récursion peut déborder la pile -> utiliser la version file ci-dessous. */
int remplir(int l, int c) {
	if (!dans_grille(l, c)) return 0;
	if (grille[l][c] == '#' || vu[l][c]) return 0;   /* ADAPTER : caractère "mur" */
	vu[l][c] = 1;
	int taille = 1;
	for (int d = 0; d < 4; d++) {
		taille += remplir(l + dl[d], c + dc[d]);
	}
	return taille;
}

/* ---- 2. compter les zones (CSES Counting Rooms) ----------------------- */
int compter_zones(int *plus_grande) {
	memset(vu, 0, sizeof(vu));
	int zones = 0;
	*plus_grande = 0;
	for (int l = 0; l < nl; l++) {
		for (int c = 0; c < nc; c++) {
			if (grille[l][c] != '#' && !vu[l][c]) {    /* case libre jamais vue = nouvelle zone */
				int taille = remplir(l, c);
				zones++;
				if (taille > *plus_grande) *plus_grande = taille;
			}
		}
	}
	return zones;
}

/* ---- 3. plus court chemin (BFS) --------------------------------------- */
/* La FILE : on y range les cases à traiter, dans l'ordre où on les découvre.
 * Comme on traite d'abord les cases à distance 1, puis 2, puis 3..., la
 * première fois qu'on atteint une case, c'est par un plus court chemin.
 * Chaque case entre au plus une fois dans la file -> taille MAXL * MAXL. */
int file_l[MAXL * MAXL], file_c[MAXL * MAXL];
int dist[MAXL][MAXL];            /* dist[l][c] = nombre de pas depuis le départ, -1 = pas atteint */
int venu_par[MAXL][MAXL];        /* direction utilisée pour ARRIVER sur la case (pour le chemin) */

void bfs(int depart_l, int depart_c) {
	for (int l = 0; l < nl; l++)
		for (int c = 0; c < nc; c++)
			dist[l][c] = -1;

	int tete = 0, queue = 0;                 /* on lit en tete, on écrit en queue */
	file_l[queue] = depart_l; file_c[queue] = depart_c; queue++;
	dist[depart_l][depart_c] = 0;

	while (tete < queue) {                   /* tant que la file n'est pas vide */
		int l = file_l[tete], c = file_c[tete];
		tete++;
		for (int d = 0; d < 4; d++) {
			int l2 = l + dl[d], c2 = c + dc[d];
			if (!dans_grille(l2, c2)) continue;
			if (grille[l2][c2] == '#') continue;      /* ADAPTER : cases interdites */
			if (dist[l2][c2] != -1) continue;         /* déjà atteinte (donc par plus court) */
			dist[l2][c2] = dist[l][c] + 1;
			venu_par[l2][c2] = d;
			file_l[queue] = l2; file_c[queue] = c2; queue++;
		}
	}
}

/* ---- 4. retrouver le chemin après un bfs ------------------------------ */
/* On part de l'arrivée et on recule case par case grâce à venu_par.
 * Écrit "DDBBG..." dans chemin[]. Renvoie la longueur, ou -1 si inaccessible. */
int reconstruire_chemin(int arrivee_l, int arrivee_c, char chemin[]) {
	int longueur = dist[arrivee_l][arrivee_c];
	if (longueur == -1) return -1;
	chemin[longueur] = '\0';
	int l = arrivee_l, c = arrivee_c;
	for (int k = longueur - 1; k >= 0; k--) {
		int d = venu_par[l][c];
		chemin[k] = nom_direction[d];
		l -= dl[d];                          /* on recule dans la direction opposée */
		c -= dc[d];
	}
	return longueur;
}

/* ---- 5. trouver une case précise (départ 'A', arrivée 'B'...) --------- */
int trouver_case(char cible, int *l_trouve, int *c_trouve) {
	for (int l = 0; l < nl; l++) {
		for (int c = 0; c < nc; c++) {
			if (grille[l][c] == cible) { *l_trouve = l; *c_trouve = c; return 1; }
		}
	}
	return 0;
}

/* ---- 6. alignements : k cases identiques à la suite (puissance 4...) -- */
/* On teste 4 directions seulement (droite, bas, 2 diagonales) : les 4
 * autres sont les mêmes alignements parcourus à l'envers.
 * Renvoie 1 si le joueur `pion` a k pions alignés. */
int a_aligne(char pion, int k) {
	const int al[4] = {0, 1, 1, 1};          /* droite, bas, diagonale \, diagonale / */
	const int ac[4] = {1, 0, 1, -1};
	for (int l = 0; l < nl; l++) {
		for (int c = 0; c < nc; c++) {
			for (int d = 0; d < 4; d++) {
				int longueur = 0;
				int l2 = l, c2 = c;
				while (longueur < k && dans_grille(l2, c2) && grille[l2][c2] == pion) {
					longueur++;
					l2 += al[d];
					c2 += ac[d];
				}
				if (longueur == k) return 1;
			}
		}
	}
	return 0;
}

/* ---- 7. compter les voisins d'un certain type (jeu de la vie...) ------ */
int nb_voisins(int l, int c, char type) {
	int compte = 0;
	for (int d = 0; d < 4; d++) {
		int l2 = l + dl[d], c2 = c + dc[d];
		if (dans_grille(l2, c2) && grille[l2][c2] == type) compte++;
	}
	return compte;
}

void charger(int lignes, int colonnes, const char *source[]) {
	nl = lignes; nc = colonnes;
	for (int l = 0; l < nl; l++) strcpy(grille[l], source[l]);
}

int main(int argc, char *argv[]) {
	if (argc > 1 && strcmp(argv[1], "p4") == 0) {
		/* grille sans dimensions sur l'entrée standard, pions J et R */
		nl = 0;
		while (scanf("%1000s", grille[nl]) == 1) nl++;
		nc = (int)strlen(grille[0]);
		if (a_aligne('J', 4)) printf("J\n");
		else if (a_aligne('R', 4)) printf("R\n");
		else printf("aucun\n");
		return 0;
	}

	const char *pieces[] = {
		"########",
		"#..#...#",
		"####.#.#",
		"#..#...#",
		"########",
	};
	charger(5, 8, pieces);
	int plus_grande;
	int zones = compter_zones(&plus_grande);
	printf("zones=%d plus grande=%d (attendu 3 et 8)\n", zones, plus_grande);

	const char *labyrinthe[] = {
		"A.#.",
		"..#B",
		"....",
	};
	charger(3, 4, labyrinthe);
	int al, ac, bl, bc;
	trouver_case('A', &al, &ac);
	trouver_case('B', &bl, &bc);
	bfs(al, ac);
	char chemin[MAXL];
	int longueur = reconstruire_chemin(bl, bc, chemin);
	printf("distance A->B=%d chemin=%s (attendu 6, un chemin de 6 lettres)\n", longueur, chemin);
	printf("voisins libres de (2,1) : %d (attendu 3)\n", nb_voisins(2, 1, '.'));

	const char *jeu[] = {
		"0000J00",
		"000JR00",
		"00JRR00",
		"0JRRR00",
	};
	charger(4, 7, jeu);
	printf("J aligne 4 : %d (attendu 1), R aligne 4 : %d (attendu 0)\n", a_aligne('J', 4), a_aligne('R', 4));
	return 0;
}
