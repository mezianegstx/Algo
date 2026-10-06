/*
 * Rattrapage 2022 - Problème 1 : Nombre le plus fréquent (4 points)
 * =================================================================
 * ENONCE : n puis n entiers relatifs. Afficher celui qui a le plus
 *   d'occurrences. En cas d'égalité, afficher le PLUS GRAND.
 *   Exemple 1 : 1 2 3 4 5 2 2 3 4 5 -> 2  (présent 3 fois)
 *   Exemple 2 : 1 2 3 4 5 1 2 3 4 5 -> 5  (tous 2 fois, le plus grand)
 *   Exemple 3 : 10 9 8 ... 1        -> 10 (tous 1 fois, le plus grand)
 *
 * IDEE : TRIER puis compter les "paquets" de valeurs égales.
 *   Après tri, les valeurs égales sont côte à côte : 1 2 2 2 3 3 4 4 5 5
 *   On parcourt et on mesure la longueur de chaque paquet.
 *   On garde le meilleur avec ">=" : comme le tableau est trié par ordre
 *   CROISSANT, en cas d'égalité le paquet vu en dernier est le plus grand
 *   -> c'est exactement la règle demandée.
 *   Complexité O(n log n), marche quelles que soient les valeurs (négatives,
 *   très grandes), contrairement à un tableau de compteurs indexé par valeur.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Egalité : ">=" (le plus grand gagne). Avec ">" on aurait le plus petit.
 *   - Entiers RELATIFS : un tableau compte[valeur] planterait sur -5
 *     (indice négatif). Le tri évite ce problème.
 *   - Comparateur qsort : ne pas faire "return a - b" (débordement si
 *     a = 2 000 000 000 et b = -2 000 000 000). Faire des comparaisons.
 *   - n = 1 : on affiche ce nombre.
 *   - n inconnu à l'avance -> malloc.
 *   - Ne pas oublier le DERNIER paquet (géré ici car on traite chaque
 *     paquet entièrement dans la boucle while interne).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int comparer_int(const void *a, const void *b)
{
    int x = *(const int *)a;
    int y = *(const int *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

int main(void)
{
    int n, i;
    if (scanf("%d", &n) != 1 || n <= 0) return 0;

    int *t = malloc(n * sizeof(int));
    for (i = 0; i < n; i++) scanf("%d", &t[i]);

    qsort(t, n, sizeof(int), comparer_int);

    int meilleure_valeur = t[0];
    int meilleur_compte = 0;

    i = 0;
    while (i < n) {
        int valeur = t[i];
        int compte = 0;
        while (i < n && t[i] == valeur) {   /* longueur du paquet */
            compte++;
            i++;
        }
        if (compte >= meilleur_compte) {    /* >= : à égalité, le plus grand */
            meilleur_compte = compte;
            meilleure_valeur = valeur;
        }
    }

    printf("%d\r\n", meilleure_valeur);
    free(t);
    return 0;
}
