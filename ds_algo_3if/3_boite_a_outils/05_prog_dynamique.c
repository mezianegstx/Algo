/*
 * BOITE A OUTILS 5 : PROGRAMMATION DYNAMIQUE (DP) ET TECHNIQUES CLASSIQUES
 * =======================================================================
 * Quand l'énoncé dit "peut-on atteindre exactement", "maximum sans dépasser",
 * "nombre de façons", "minimum de pièces"... c'est presque toujours une DP
 * sur un tableau indexé par la somme/capacité.
 *
 * RECONNAITRE LE BON SAC A DOS
 *   chaque objet UNE fois     -> boucle sur s DECROISSANTE  (TD3, chèvres)
 *   objets en nombre ILLIMITE -> boucle sur s CROISSANTE    (TD2-2)
 *   "exactement C"            -> réponse possible[C]
 *   "au plus C, le maximum"   -> plus grand s <= C avec possible[s]
 *   avec des VALEURS à maximiser -> sac_a_dos_valeur()
 *
 * Toujours vérifier : taille du tableau = capacité + 1 (indices 0..C).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/* 1. Somme de sous-ensemble (chaque objet au plus une fois)                */
/*    Renvoie un tableau possible[0..C]. free après usage.                  */
/* ---------------------------------------------------------------------- */
char *subset_sum_0_1(const int *w, int n, int C)
{
    char *possible = calloc(C + 1, 1);
    int i, s;
    possible[0] = 1;
    for (i = 0; i < n; i++) {
        if (w[i] <= 0 || w[i] > C) continue;
        for (s = C; s >= w[i]; s--)               /* DECROISSANT */
            if (possible[s - w[i]]) possible[s] = 1;
    }
    return possible;
}

/* ---------------------------------------------------------------------- */
/* 2. Somme atteignable avec objets illimités                               */
/* ---------------------------------------------------------------------- */
char *subset_sum_illimite(const int *w, int n, int C)
{
    char *possible = calloc(C + 1, 1);
    int i, s;
    possible[0] = 1;
    for (i = 0; i < n; i++) {
        if (w[i] <= 0) continue;
        for (s = w[i]; s <= C; s++)               /* CROISSANT */
            if (possible[s - w[i]]) possible[s] = 1;
    }
    return possible;
}

/* ---------------------------------------------------------------------- */
/* 3. Sac à dos classique : objets (poids, valeur), capacité C,            */
/*    maximiser la valeur totale, chaque objet au plus une fois.            */
/*    best[s] = meilleure valeur avec un poids total <= s                   */
/* ---------------------------------------------------------------------- */
long long sac_a_dos_valeur(const int *poids, const int *valeur, int n, int C)
{
    long long *best = calloc(C + 1, sizeof(long long));
    int i, s;
    for (i = 0; i < n; i++) {
        for (s = C; s >= poids[i]; s--) {         /* DECROISSANT (0/1) */
            long long avec = best[s - poids[i]] + valeur[i];
            if (avec > best[s]) best[s] = avec;
        }
    }
    long long r = best[C];
    free(best);
    return r;
    /* objets illimités : même chose avec s CROISSANT de poids[i] à C */
}

/* ---------------------------------------------------------------------- */
/* 4. Rendu de monnaie : nombre MINIMUM de pièces pour faire S             */
/*    (pièces illimitées). -1 si impossible.                                */
/*    PIEGE : le glouton (plus grosse pièce d'abord) est FAUX en général :  */
/*    pièces {1, 3, 4}, S = 6 : glouton 4+1+1 (3 pièces), optimal 3+3 (2).  */
/* ---------------------------------------------------------------------- */
int rendu_monnaie_min(const int *pieces, int n, int S)
{
    int *m = malloc((S + 1) * sizeof(int));
    int i, s, INF = 1000000000;
    m[0] = 0;
    for (s = 1; s <= S; s++) m[s] = INF;
    for (s = 1; s <= S; s++) {
        for (i = 0; i < n; i++) {
            if (pieces[i] <= s && m[s - pieces[i]] != INF && m[s - pieces[i]] + 1 < m[s])
                m[s] = m[s - pieces[i]] + 1;
        }
    }
    int r = (m[S] == INF) ? -1 : m[S];
    free(m);
    return r;
}

/* 5. Nombre de FACONS de faire S avec des pièces illimitées (ordre ignoré) */
long long nb_facons_monnaie(const int *pieces, int n, int S)
{
    long long *f = calloc(S + 1, sizeof(long long));
    int i, s;
    f[0] = 1;                                     /* 1 façon de faire 0 */
    for (i = 0; i < n; i++)                       /* boucle objets DEHORS */
        for (s = pieces[i]; s <= S; s++)
            f[s] += f[s - pieces[i]];
    long long r = f[S];
    free(f);
    return r;
}

/* ---------------------------------------------------------------------- */
/* 6. Sous-tableau de somme maximale (Kadane), O(n)                        */
/*    Si tous négatifs, renvoie le plus grand élément (sous-tableau non vide) */
/* ---------------------------------------------------------------------- */
long long somme_max_sous_tableau(const int *t, int n)
{
    long long best = t[0], courant = t[0];
    int i;
    for (i = 1; i < n; i++) {
        courant = (courant > 0) ? courant + t[i] : t[i];  /* on repart si < 0 */
        if (courant > best) best = courant;
    }
    return best;
}

/* ---------------------------------------------------------------------- */
/* 7. Sommes préfixes : somme de t[a..b] en O(1) après O(n) de préparation  */
/*    P[0] = 0, P[i+1] = P[i] + t[i]  ->  somme(a..b) = P[b+1] - P[a]        */
/* ---------------------------------------------------------------------- */
long long *prefixes(const int *t, int n)
{
    long long *P = malloc((n + 1) * sizeof(long long));
    int i;
    P[0] = 0;
    for (i = 0; i < n; i++) P[i + 1] = P[i] + t[i];
    return P;
}

/* ---------------------------------------------------------------------- */
/* 8. Plus longue sous-suite strictement croissante (LIS), O(n²)            */
/* ---------------------------------------------------------------------- */
int plus_longue_croissante(const int *t, int n)
{
    int *L = malloc(n * sizeof(int));
    int i, j, best = 0;
    for (i = 0; i < n; i++) {
        L[i] = 1;
        for (j = 0; j < i; j++)
            if (t[j] < t[i] && L[j] + 1 > L[i]) L[i] = L[j] + 1;
        if (L[i] > best) best = L[i];
    }
    free(L);
    return best;
}

/* ---------------------------------------------------------------------- */
/* 9. Plus longue sous-séquence commune de deux chaînes (LCS), O(n*m)       */
/* ---------------------------------------------------------------------- */
int lcs(const char *a, const char *b)
{
    int n = (int)strlen(a), m = (int)strlen(b), i, j;
    int **d = malloc((n + 1) * sizeof(int *));
    for (i = 0; i <= n; i++) d[i] = calloc(m + 1, sizeof(int));
    for (i = 1; i <= n; i++)
        for (j = 1; j <= m; j++)
            if (a[i - 1] == b[j - 1]) d[i][j] = d[i - 1][j - 1] + 1;
            else d[i][j] = (d[i - 1][j] > d[i][j - 1]) ? d[i - 1][j] : d[i][j - 1];
    int r = d[n][m];
    for (i = 0; i <= n; i++) free(d[i]);
    free(d);
    return r;
}

/* ---------------------------------------------------------------------- */
/* 10. Deux pointeurs : dans un tableau TRIE, existe-t-il i < j avec       */
/*     t[i] + t[j] == cible ? O(n)                                          */
/* ---------------------------------------------------------------------- */
int paire_de_somme(const int *t, int n, int cible)
{
    int i = 0, j = n - 1;
    while (i < j) {
        int s = t[i] + t[j];
        if (s == cible) return 1;
        if (s < cible) i++;
        else           j--;
    }
    return 0;
}

int main(void)
{
    int w[] = {20, 3, 5, 1};
    char *p = subset_sum_0_1(w, 4, 24);
    printf("%s\r\n", p[24] ? "OUI" : "NON");
    free(p);
    int pieces[] = {1, 3, 4};
    printf("%d\r\n", rendu_monnaie_min(pieces, 3, 6));
    return 0;
}
