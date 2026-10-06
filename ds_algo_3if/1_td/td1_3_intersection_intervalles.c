/*
 * TD1-3 : Aire de l'intersection de deux intervalles
 * ==================================================
 * ENONCE : lire a1, b1, a2, b2 (réels), afficher la longueur de
 *   [a1;b1] ∩ [a2;b2].
 *   Exemple 1 : [13;35] et [15;40] -> 20   (intersection [15;35])
 *   Exemple 2 : [1;10]  et [13;15] -> 0    (pas d'intersection)
 *
 * IDEE (formule à retenir) :
 *   début de l'intersection = max(a1, a2)
 *   fin de l'intersection   = min(b1, b2)
 *   longueur = fin - début, et si c'est négatif -> 0 (intervalles disjoints)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Intervalles disjoints : fin < début -> afficher 0 (pas un nombre négatif !)
 *   - Intervalles qui se touchent ([1;5] et [5;9]) : longueur 0.
 *   - Un intervalle inclus dans l'autre ([0;10] et [2;3]) : 1, la formule marche.
 *   - Si jamais une borne est donnée à l'envers (a > b), on échange par sécurité.
 *   - Réels : lecture "%lf", affichage sans ".000000" (voir afficher_reel).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void afficher_reel(double x)
{
    long long entier = (long long)x;
    if (x == (double)entier) printf("%lld\r\n", entier);
    else                     printf("%.10g\r\n", x);
}

double max2(double x, double y) { return (x > y) ? x : y; }
double min2(double x, double y) { return (x < y) ? x : y; }

int main(void)
{
    double a1, b1, a2, b2, tmp;
    scanf("%lf", &a1);
    scanf("%lf", &b1);
    scanf("%lf", &a2);
    scanf("%lf", &b2);

    /* sécurité : remettre les bornes dans l'ordre */
    if (a1 > b1) { tmp = a1; a1 = b1; b1 = tmp; }
    if (a2 > b2) { tmp = a2; a2 = b2; b2 = tmp; }

    double debut = max2(a1, a2);
    double fin   = min2(b1, b2);
    double aire  = fin - debut;
    if (aire < 0) aire = 0;      /* intervalles disjoints */

    afficher_reel(aire);
    return 0;
}
