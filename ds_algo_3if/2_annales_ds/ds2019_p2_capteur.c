/*
 * DS mai 2019 - Problème 2 : Capteur (cohérence des valeurs)
 * ==========================================================
 * ENONCE : n (multiple de 10, 10 <= n <= 1 000 000) puis n réels.
 *   m1 = moyenne des 10% plus GRANDES et des 10% plus PETITES valeurs
 *   m2 = moyenne de toutes les valeurs
 *   |m1 - m2| < 2 (strictement) -> "OUI", sinon "NON".
 *   Exemple : 17 18 17 14 15 20 10 20 10 18
 *     10% de 10 = 1 valeur de chaque côté : m1 = (10 + 20) / 2 = 15
 *     m2 = 156 / 10 = 15.6 -> écart 0.6 < 2 -> OUI
 *
 * IDEE :
 *   1. stocker les n valeurs (malloc car jusqu'à 1 million)
 *   2. trier (qsort, O(n log n), largement assez rapide)
 *   3. k = n / 10. Les k plus petites sont t[0..k-1], les k plus grandes
 *      t[n-k..n-1].  m1 = (somme des 2k valeurs) / (2k)
 *   4. m2 = somme totale / n
 *   5. comparer fabs(m1 - m2) < 2
 *
 * CAS PARTICULIERS / PIEGES :
 *   - "différence" = valeur ABSOLUE (m1 peut être plus grand ou plus petit
 *     que m2) -> fabs, et #include <math.h>.
 *   - STRICTEMENT < 2 : un écart pile de 2 -> NON.
 *   - m1 se divise par 2k (pas par k, ni par n !).
 *   - Valeurs en double (ex : deux fois 20) : le tri gère, on prend bien
 *     k valeurs de chaque côté, pas "les valeurs distinctes".
 *   - n = 10 : k = 1, une valeur de chaque côté.
 *   - 1 million de double = 8 Mo : malloc, PAS de tableau local (pile).
 *   - Comparateur de double pour qsort : ne PAS faire return a - b (cast en
 *     int faux pour 0.5). Utiliser des comparaisons (voir comparer_double).
 *   - Lecture "%lf" pour les double.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int comparer_double(const void *a, const void *b)
{
    double x = *(const double *)a;
    double y = *(const double *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main(void)
{
    int n, i;
    scanf("%d", &n);

    double *t = malloc(n * sizeof(double));
    double somme_totale = 0;

    for (i = 0; i < n; i++) {
        scanf("%lf", &t[i]);
        somme_totale += t[i];
    }

    qsort(t, n, sizeof(double), comparer_double);

    int k = n / 10;                     /* nombre de valeurs dans 10% */
    double somme_extremes = 0;
    for (i = 0; i < k; i++) {
        somme_extremes += t[i];         /* les k plus petites */
        somme_extremes += t[n - 1 - i]; /* les k plus grandes */
    }

    double m1 = somme_extremes / (2.0 * k);
    double m2 = somme_totale / n;

    if (fabs(m1 - m2) < 2) printf("OUI\r\n");
    else                   printf("NON\r\n");

    free(t);
    return 0;
}
