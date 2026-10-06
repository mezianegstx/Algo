/*
 * Rattrapage 2022 - Problème 3 : Nombres "forts"
 * ==============================================
 * ENONCE : N est "fort" si la somme de ses diviseurs NON-TRIVIAUX (tous les
 *   diviseurs sauf 1 et N) est >= N. Compter les nombres forts <= LIM
 *   (1 < LIM <= 100 000).
 *   Exemple : LIM = 20 -> 3
 *     12 : 2+3+4+6 = 15 >= 12      18 : 2+3+6+9 = 20 >= 18
 *     20 : 2+4+5+10 = 21 >= 20
 *
 * IDEE 1 (simple, assez rapide) : pour chaque N, chercher ses diviseurs
 *   jusqu'à racine(N) : si d divise N, alors d ET N/d sont diviseurs.
 *   Coût total ≈ 100 000 * 316 = 32 millions d'opérations : OK.
 *
 * IDEE 2 (crible, plus rapide, utilisée dans main) : au lieu de chercher les
 *   diviseurs de chaque N, chaque d "distribue" sa valeur à ses multiples :
 *   pour d de 2 à LIM, pour m = 2d, 3d, 4d... <= LIM : somme[m] += d
 *   On commence à d = 2 (pas 1) et à m = 2d (pas d) : on exclut donc bien
 *   les diviseurs triviaux 1 et N. Coût ≈ LIM * ln(LIM), très rapide.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Exclure 1 ET N lui-même de la somme.
 *   - Carré parfait (ex 16 = 4*4) dans l'idée 1 : ne pas ajouter 4 deux fois
 *     (test d != N/d).
 *   - Nombres premiers : aucun diviseur non trivial, somme 0 -> jamais forts.
 *   - "supérieure OU EGALE" : >= et pas >.
 *   - LIM inclus ("inférieurs ou égaux") : boucle jusqu'à <= LIM.
 *   - Tableau de 100 001 cases : en global (pas sur la pile).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAXLIM 100000

long long somme[MAXLIM + 1];   /* global = initialisé à 0 */

/* IDEE 1 : somme des diviseurs non triviaux de n, en O(racine(n)) */
long long somme_diviseurs_non_triviaux(int n)
{
    long long s = 0;
    int d;
    for (d = 2; (long long)d * d <= n; d++) {
        if (n % d == 0) {
            s += d;                      /* le petit diviseur      */
            if (d != n / d) s += n / d;  /* le grand, sauf carré   */
        }
    }
    return s;
}

int main(void)
{
    int LIM, d, m, N, compteur = 0;
    scanf("%d", &LIM);
    if (LIM > MAXLIM) LIM = MAXLIM;

    /* IDEE 2 : crible */
    for (d = 2; d <= LIM; d++) {
        for (m = 2 * d; m <= LIM; m += d) {
            somme[m] += d;
        }
    }

    for (N = 1; N <= LIM; N++) {
        if (somme[N] >= N) compteur++;
        /* équivalent : if (somme_diviseurs_non_triviaux(N) >= N) compteur++; */
    }

    printf("%d\r\n", compteur);
    return 0;
}
