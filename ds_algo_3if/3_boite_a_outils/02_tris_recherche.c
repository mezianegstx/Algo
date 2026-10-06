/*
 * BOITE A OUTILS 2 : TRIS ET RECHERCHE
 * ====================================
 * qsort est dans stdlib.h (bibliothèque C standard, PAS la STL) : autorisé.
 * Utiliser qsort en priorité. Les tris "à la main" sont là si on demande
 * explicitement d'en écrire un.
 *
 *   qsort(tableau, nombre_elements, sizeof(un_element), comparateur);
 *   Le comparateur renvoie < 0 si a avant b, 0 si égaux, > 0 si a après b.
 *
 * PIEGE : "return a - b" déborde pour de grands int, et est FAUX pour des
 *   double (0.5 converti en int = 0). Toujours faire des comparaisons.
 *
 * COMPLEXITES
 *   qsort, tri fusion : O(n log n)   -> OK jusqu'à plusieurs millions
 *   tri insertion/sélection/bulles : O(n²) -> OK jusqu'à ~10 000
 *   recherche dichotomique : O(log n) (tableau TRIE obligatoire)
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/* Comparateurs pour qsort                                                 */
/* ---------------------------------------------------------------------- */
int cmp_int_croissant(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);          /* -1, 0 ou 1, sans débordement */
}

int cmp_int_decroissant(const void *a, const void *b)
{
    return cmp_int_croissant(b, a);    /* on inverse a et b */
}

int cmp_double(const void *a, const void *b)
{
    double x = *(const double *)a, y = *(const double *)b;
    return (x > y) - (x < y);
}

/* Tableau de chaînes : char mots[N][100] */
int cmp_chaines_tableau(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}
/* usage : qsort(mots, n, sizeof(mots[0]), cmp_chaines_tableau); */

/* Tableau de pointeurs : char *mots[N] */
int cmp_chaines_pointeurs(const void *a, const void *b)
{
    return strcmp(*(char *const *)a, *(char *const *)b);
}

/* Structures : tri sur plusieurs critères */
typedef struct {
    char nom[50];
    int note;
} Eleve;

int cmp_eleves(const void *a, const void *b)
{
    const Eleve *e1 = (const Eleve *)a, *e2 = (const Eleve *)b;
    if (e1->note != e2->note) return e2->note - e1->note; /* note décroissante */
    return strcmp(e1->nom, e2->nom);                       /* puis nom A-Z     */
}

/* ---------------------------------------------------------------------- */
/* Tri par insertion (simple, O(n²), stable)                               */
/* Idée : comme trier des cartes en main, on insère t[i] à sa place parmi  */
/* t[0..i-1] déjà triés en décalant les plus grands vers la droite.        */
/* ---------------------------------------------------------------------- */
void tri_insertion(int *t, int n)
{
    int i, j;
    for (i = 1; i < n; i++) {
        int x = t[i];
        j = i - 1;
        while (j >= 0 && t[j] > x) {
            t[j + 1] = t[j];
            j--;
        }
        t[j + 1] = x;
    }
}

/* ---------------------------------------------------------------------- */
/* Tri par sélection (O(n²)) : on cherche le min et on le met devant       */
/* ---------------------------------------------------------------------- */
void tri_selection(int *t, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++) {
        int imin = i;
        for (j = i + 1; j < n; j++) if (t[j] < t[imin]) imin = j;
        int tmp = t[i]; t[i] = t[imin]; t[imin] = tmp;
    }
}

/* ---------------------------------------------------------------------- */
/* Tri fusion (O(n log n), stable) : couper en deux, trier, fusionner      */
/* ---------------------------------------------------------------------- */
static void fusionner(int *t, int *tmp, int g, int m, int d)
{
    int i = g, j = m + 1, k = g;
    while (i <= m && j <= d) tmp[k++] = (t[i] <= t[j]) ? t[i++] : t[j++];
    while (i <= m) tmp[k++] = t[i++];
    while (j <= d) tmp[k++] = t[j++];
    for (k = g; k <= d; k++) t[k] = tmp[k];
}

static void tri_fusion_rec(int *t, int *tmp, int g, int d)
{
    if (g >= d) return;
    int m = (g + d) / 2;
    tri_fusion_rec(t, tmp, g, m);
    tri_fusion_rec(t, tmp, m + 1, d);
    fusionner(t, tmp, g, m, d);
}

void tri_fusion(int *t, int n)
{
    if (n <= 1) return;
    int *tmp = malloc(n * sizeof(int));
    tri_fusion_rec(t, tmp, 0, n - 1);
    free(tmp);
}

/* ---------------------------------------------------------------------- */
/* Tri rapide (quicksort) à la main, pivot = élément du milieu             */
/* ---------------------------------------------------------------------- */
void tri_rapide(int *t, int g, int d)   /* appel : tri_rapide(t, 0, n - 1) */
{
    if (g >= d) return;
    int pivot = t[(g + d) / 2];
    int i = g, j = d;
    while (i <= j) {
        while (t[i] < pivot) i++;
        while (t[j] > pivot) j--;
        if (i <= j) {
            int tmp = t[i]; t[i] = t[j]; t[j] = tmp;
            i++; j--;
        }
    }
    tri_rapide(t, g, j);
    tri_rapide(t, i, d);
}

/* ---------------------------------------------------------------------- */
/* Tri par comptage : O(n + plage), quand les valeurs sont petites (0..K)  */
/* ---------------------------------------------------------------------- */
void tri_comptage(int *t, int n, int K)
{
    int *compte = calloc(K + 1, sizeof(int));
    int i, v, k = 0;
    for (i = 0; i < n; i++) compte[t[i]]++;
    for (v = 0; v <= K; v++) while (compte[v]-- > 0) t[k++] = v;
    free(compte);
}

/* ---------------------------------------------------------------------- */
/* Recherche dichotomique dans un tableau TRIE croissant                   */
/* Renvoie l'indice de x, ou -1 s'il est absent.                           */
/* ---------------------------------------------------------------------- */
int recherche_dicho(const int *t, int n, int x)
{
    int g = 0, d = n - 1;
    while (g <= d) {
        int m = g + (d - g) / 2;      /* évite le débordement de (g+d) */
        if (t[m] == x) return m;
        if (t[m] < x) g = m + 1;
        else          d = m - 1;
    }
    return -1;
}

/* Premier indice i tel que t[i] >= x (n si aucun) : "lower bound" */
int premier_superieur_ou_egal(const int *t, int n, int x)
{
    int g = 0, d = n;                 /* intervalle [g, d[ */
    while (g < d) {
        int m = g + (d - g) / 2;
        if (t[m] < x) g = m + 1;
        else          d = m;
    }
    return g;
}
/* Nombre d'occurrences de x dans un tableau trié :
   premier_superieur_ou_egal(t, n, x + 1) - premier_superieur_ou_egal(t, n, x) */

/* ---------------------------------------------------------------------- */
/* Recherche de min / max / position en un passage                         */
/* ---------------------------------------------------------------------- */
int indice_du_max(const int *t, int n)
{
    int i, imax = 0;
    for (i = 1; i < n; i++) if (t[i] > t[imax]) imax = i;  /* > : 1re occurrence */
    return imax;                                           /* >= : dernière     */
}

/* ---------------------------------------------------------------------- */
/* Supprimer les doublons d'un tableau TRIE (renvoie la nouvelle taille)   */
/* ---------------------------------------------------------------------- */
int supprimer_doublons_trie(int *t, int n)
{
    int i, k = 0;
    for (i = 0; i < n; i++) {
        if (k == 0 || t[i] != t[k - 1]) t[k++] = t[i];
    }
    return k;
}

int main(void)
{
    int t[] = {5, -2, 9, 1, 5, 6};
    int n = 6, i;
    qsort(t, n, sizeof(int), cmp_int_croissant);
    for (i = 0; i < n; i++) printf("%d ", t[i]);
    printf("\r\nindice de 6 : %d\r\n", recherche_dicho(t, n, 6));
    return 0;
}
