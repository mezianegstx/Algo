/*
 * TD1-5 : Conjecture de Collatz (suite de Syracuse)
 * =================================================
 * ENONCE : lire n (< 1000), afficher les termes de la suite
 *   a0 = n, a(i) = a(i-1)/2 si pair, 3*a(i-1)+1 si impair,
 *   jusqu'au premier 1 INCLUS (l'exemple affiche le 1 à la fin).
 *   Exemple : 6 -> 6 3 10 5 16 8 4 2 1 (un par ligne)
 *
 * IDEE : boucle tant que n != 1 : afficher n, puis appliquer f.
 *   Après la boucle, afficher le 1 final.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - n = 1 : on affiche juste "1" (la boucle ne tourne pas).
 *   - Les valeurs MONTENT avant de redescendre (27 monte à 9232).
 *     Pour n < 1000 le max est 250504 : un int suffit, mais long long
 *     par sécurité si l'énoncé change de borne.
 *   - n <= 0 : la suite ne termine jamais (0 -> 0 -> 0...). On protège
 *     en n'affichant rien d'autre que n (cas hors énoncé).
 *   - Si on demande la LONGUEUR du cycle au lieu des termes : compter les
 *     tours de boucle (voir longueur_cycle()).
 *   - Test pair : n % 2 == 0.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/* BONUS : nombre de termes avant le premier 1 */
int longueur_cycle(long long n)
{
    int compteur = 0;
    while (n > 1) {
        if (n % 2 == 0) n = n / 2;
        else            n = 3 * n + 1;
        compteur++;
    }
    return compteur;
}

int main(void)
{
    long long n;
    scanf("%lld", &n);

    if (n <= 0) {                 /* hors énoncé : éviter une boucle infinie */
        printf("%lld\r\n", n);
        return 0;
    }

    while (n != 1) {
        printf("%lld\r\n", n);
        if (n % 2 == 0) n = n / 2;       /* pair   */
        else            n = 3 * n + 1;   /* impair */
    }
    printf("1\r\n");                     /* le 1 final est affiché */
    return 0;
}
