/*
 * TD1-6 : Fusion d'intervalles (ensemble équivalent d'aire minimale)
 * ==================================================================
 * ENONCE : lire n puis n intervalles [x y]. Produire l'ensemble équivalent
 *   (mêmes points) sans chevauchement, trié par borne gauche, précédé de
 *   son nombre d'intervalles.
 *   Exemple : [-8,2] [1,5] [7,9] -> 2 / -8 5 / 7 9
 *
 * IDEE (algo classique "merge intervals") :
 *   1. Trier les intervalles par borne gauche (qsort).
 *   2. Parcourir : on garde un intervalle "courant" [deb, fin].
 *      - si le suivant commence avant (ou pile à) la fin courante : il
 *        chevauche -> on étend : fin = max(fin, sa fin)
 *      - sinon : on enregistre le courant et le suivant devient le courant.
 *   3. Ne pas oublier d'enregistrer le DERNIER courant à la fin.
 *   Complexité : O(n log n) à cause du tri.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Intervalles qui se TOUCHENT ([1,3] et [3,5]) : ils partagent le point 3,
 *     on les fusionne -> [1,5]. D'où le test "<=" et pas "<".
 *   - Intervalle inclus dans un autre ([1,10] puis [2,3]) : fin = max(10,3)
 *     = 10. Si on écrit fin = 3 au lieu du max, c'est FAUX.
 *   - Entrée non triée : le tri règle tout.
 *   - n = 0 : afficher "0".
 *   - Bornes réelles possibles : on lit des double, affichage sans ".000".
 *   - Borne donnée à l'envers (x > y) : on échange par sécurité.
 *   - n inconnu à l'avance -> malloc (pas de tableau fixe trop petit).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    double deb;
    double fin;
} Intervalle;

/* Comparateur pour qsort : tri par borne gauche, puis par borne droite */
int comparer(const void *a, const void *b)
{
    const Intervalle *ia = (const Intervalle *)a;
    const Intervalle *ib = (const Intervalle *)b;
    if (ia->deb < ib->deb) return -1;
    if (ia->deb > ib->deb) return 1;
    if (ia->fin < ib->fin) return -1;
    if (ia->fin > ib->fin) return 1;
    return 0;
}

void afficher_nombre(double x)   /* sans \r\n */
{
    long long entier = (long long)x;
    if (x == (double)entier) printf("%lld", entier);
    else                     printf("%.10g", x);
}

int main(void)
{
    int n, i;
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("0\r\n");
        return 0;
    }

    Intervalle *t   = malloc(n * sizeof(Intervalle));
    Intervalle *res = malloc(n * sizeof(Intervalle));

    for (i = 0; i < n; i++) {
        scanf("%lf %lf", &t[i].deb, &t[i].fin);
        if (t[i].deb > t[i].fin) {               /* sécurité */
            double tmp = t[i].deb; t[i].deb = t[i].fin; t[i].fin = tmp;
        }
    }

    qsort(t, n, sizeof(Intervalle), comparer);

    int nb = 0;
    Intervalle courant = t[0];
    for (i = 1; i < n; i++) {
        if (t[i].deb <= courant.fin) {
            /* chevauche ou touche : on étend (MAX, pas juste t[i].fin) */
            if (t[i].fin > courant.fin) courant.fin = t[i].fin;
        } else {
            res[nb++] = courant;   /* trou entre les deux : on valide */
            courant = t[i];
        }
    }
    res[nb++] = courant;           /* ne pas oublier le dernier ! */

    printf("%d\r\n", nb);
    for (i = 0; i < nb; i++) {
        afficher_nombre(res[i].deb);
        printf(" ");
        afficher_nombre(res[i].fin);
        printf("\r\n");
    }

    free(t);
    free(res);
    return 0;
}
