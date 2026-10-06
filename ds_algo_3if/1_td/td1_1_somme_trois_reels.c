/*
 * TD1-1 : Somme de trois nombres réels
 * =====================================
 * ENONCE : lire a, b, c (réels, un par ligne), afficher a + b + c.
 *   Exemple : 10 / -5 / 1  ->  6
 *
 * IDEE : trivial, la seule difficulté est l'AFFICHAGE d'un réel.
 *   printf("%f") afficherait "6.000000" alors que l'exemple attend "6".
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Utiliser double (plus précis que float).
 *     scanf : "%lf" pour un double ("%f" = float, sinon valeur fausse !)
 *     printf : "%f" ou "%g" marchent pour les deux.
 *   - Résultat entier (6)    -> on affiche "6".
 *   - Résultat décimal (2.5) -> on affiche "2.5".
 *   - Erreur d'arrondi : 0.1 + 0.2 = 0.30000000000000004 en machine.
 *     "%.10g" garde 10 chiffres significatifs -> affiche "0.3".
 *   - Si le juge refuse, essayer "%.2f" ou "%f" (selon ce qu'il attend).
 *   - Toujours finir la ligne par "\r\n" et ne pas oublier return 0.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/* Affiche un réel sans zéros inutiles, suivi de \r\n */
void afficher_reel(double x)
{
    long long entier = (long long)x;
    if (x == (double)entier) {
        printf("%lld\r\n", entier);   /* valeur entière : "6"   */
    } else {
        printf("%.10g\r\n", x);       /* valeur décimale : "2.5" */
    }
}

int main(void)
{
    double a, b, c;
    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%lf", &c);

    afficher_reel(a + b + c);
    return 0;
}
