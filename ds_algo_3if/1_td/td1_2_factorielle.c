/*
 * TD1-2 : n! (factorielle)
 * ========================
 * ENONCE : lire n (entier <= 12), afficher n! = 1 * 2 * ... * n.
 *   Exemple : 5 -> 120
 *
 * IDEE : une boucle qui multiplie. (La version récursive est donnée en bonus.)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - 0! = 1 (et 1! = 1) : en partant de resultat = 1, la boucle ne fait
 *     rien et on affiche bien 1. NE PAS partir de 0 !
 *   - Débordement : 12! = 479 001 600 tient dans un int (max ~2,1 milliards),
 *     mais 13! = 6 227 020 800 NON. On utilise long long par sécurité
 *     (tient jusqu'à 20!). Affichage d'un long long : "%lld".
 *   - n négatif : pas défini, on affiche 1 (aucune itération).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/* Version itérative (la plus sûre) */
long long factorielle(int n)
{
    long long resultat = 1;
    int i;
    for (i = 2; i <= n; i++) {
        resultat = resultat * i;
    }
    return resultat;
}

/* Version récursive (au cas où on demande "récursif") */
long long factorielle_rec(int n)
{
    if (n <= 1) return 1;               /* cas de base : 0! = 1! = 1 */
    return n * factorielle_rec(n - 1);  /* n! = n * (n-1)!           */
}

int main(void)
{
    int n;
    scanf("%d", &n);
    printf("%lld\r\n", factorielle(n));
    return 0;
}
