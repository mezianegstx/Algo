/*
 * BOITE A OUTILS 7 : RECURSIVITE, BACKTRACKING, GRAPHES / GRILLES
 * ===============================================================
 * Moins fréquent dans les annales, mais utile si un sujet sort du lot.
 *
 * BACKTRACKING : explorer tous les choix, revenir en arrière.
 *   n éléments -> 2^n sous-ensembles (OK jusqu'à n ~ 20-25)
 *               -> n! permutations   (OK jusqu'à n ~ 10)
 *   Au-delà : chercher une DP (voir 05_prog_dynamique.c).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/* Tous les sous-ensembles (via un masque de bits)                         */
/* bit i de masque à 1 = l'élément i est pris                              */
/* ---------------------------------------------------------------------- */
int compter_sous_ensembles_de_somme(const int *t, int n, int cible)
{
    int masque, i, nb = 0;
    for (masque = 0; masque < (1 << n); masque++) {
        long long s = 0;
        for (i = 0; i < n; i++) if (masque & (1 << i)) s += t[i];
        if (s == cible) nb++;
    }
    return nb;
}

/* ---------------------------------------------------------------------- */
/* Permutations (échange + récursion)                                      */
/* ---------------------------------------------------------------------- */
static void echanger(int *a, int *b) { int x = *a; *a = *b; *b = x; }

void permutations(int *t, int k, int n)    /* appel : permutations(t, 0, n) */
{
    int i;
    if (k == n) {
        for (i = 0; i < n; i++) printf(i ? " %d" : "%d", t[i]);
        printf("\r\n");
        return;
    }
    for (i = k; i < n; i++) {
        echanger(&t[k], &t[i]);
        permutations(t, k + 1, n);
        echanger(&t[k], &t[i]);           /* on remet comme avant */
    }
}
/* Pour l'ordre lexicographique : trier t avant, et utiliser un tableau
   "utilise[]" plutôt que des échanges. */

/* ---------------------------------------------------------------------- */
/* Récursivité classique                                                   */
/* ---------------------------------------------------------------------- */
int somme_rec(const int *t, int n)  { return n == 0 ? 0 : t[n - 1] + somme_rec(t, n - 1); }
void hanoi(int n, char de, char vers, char via)
{
    if (n == 0) return;
    hanoi(n - 1, de, via, vers);
    printf("%c %c\r\n", de, vers);
    hanoi(n - 1, via, vers, de);
}

/* ---------------------------------------------------------------------- */
/* GRILLE : parcours en largeur (BFS) = plus court chemin en nb de cases   */
/* grille[l][c] : '.' libre, '#' mur. Déplacements haut/bas/gauche/droite. */
/* Renvoie la distance de (l0,c0) à (l1,c1), ou -1 si inaccessible.        */
/* ---------------------------------------------------------------------- */
#define LMAX 100
#define CMAX 100
int bfs_grille(char g[LMAX][CMAX + 1], int L, int C, int l0, int c0, int l1, int c1)
{
    static int dist[LMAX][CMAX];
    static int file_l[LMAX * CMAX], file_c[LMAX * CMAX];
    int dl[4] = {-1, 1, 0, 0}, dc[4] = {0, 0, -1, 1};
    int i, j, debut = 0, fin = 0, k;

    for (i = 0; i < L; i++) for (j = 0; j < C; j++) dist[i][j] = -1;
    dist[l0][c0] = 0;
    file_l[fin] = l0; file_c[fin] = c0; fin++;

    while (debut < fin) {
        int l = file_l[debut], c = file_c[debut]; debut++;
        if (l == l1 && c == c1) return dist[l][c];
        for (k = 0; k < 4; k++) {
            int nl = l + dl[k], nc = c + dc[k];
            if (nl < 0 || nl >= L || nc < 0 || nc >= C) continue;  /* hors grille */
            if (g[nl][nc] == '#' || dist[nl][nc] != -1) continue;  /* mur / vu   */
            dist[nl][nc] = dist[l][c] + 1;
            file_l[fin] = nl; file_c[fin] = nc; fin++;
        }
    }
    return -1;
}

/* Remplissage (DFS récursif) : compter les zones connexes de '.' */
static void remplir(char g[LMAX][CMAX + 1], int L, int C, int l, int c)
{
    if (l < 0 || l >= L || c < 0 || c >= C || g[l][c] != '.') return;
    g[l][c] = 'x';                       /* marquer visité */
    remplir(g, L, C, l + 1, c);
    remplir(g, L, C, l - 1, c);
    remplir(g, L, C, l, c + 1);
    remplir(g, L, C, l, c - 1);
}

int compter_zones(char g[LMAX][CMAX + 1], int L, int C)
{
    int i, j, nb = 0;
    for (i = 0; i < L; i++)
        for (j = 0; j < C; j++)
            if (g[i][j] == '.') { nb++; remplir(g, L, C, i, j); }
    return nb;
}

/* ---------------------------------------------------------------------- */
/* GRAPHE en matrice d'adjacence : BFS depuis un sommet                    */
/* ---------------------------------------------------------------------- */
#define NMAX 1000
void bfs_graphe(int adj[NMAX][NMAX], int n, int depart, int *dist)
{
    int file[NMAX], debut = 0, fin = 0, u, v;
    for (v = 0; v < n; v++) dist[v] = -1;
    dist[depart] = 0;
    file[fin++] = depart;
    while (debut < fin) {
        u = file[debut++];
        for (v = 0; v < n; v++) {
            if (adj[u][v] && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                file[fin++] = v;
            }
        }
    }
}

int main(void)
{
    int t[] = {1, 2, 3};
    permutations(t, 0, 3);
    printf("%d\r\n", compter_sous_ensembles_de_somme(t, 3, 3));   /* {3}, {1,2} -> 2 */

    char g[LMAX][CMAX + 1] = {"..#", ".#.", "..."};
    printf("%d\r\n", bfs_grille(g, 3, 3, 0, 0, 1, 2));             /* 5 */
    return 0;
}
