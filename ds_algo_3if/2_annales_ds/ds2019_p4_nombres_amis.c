/*
 * DS 2019 - Problème 4 : Nombres amis (5 points)
 * ==============================================
 * ENONCE : f(N) = somme des chiffres de N. g(N) = f appliquée 100 000 fois.
 *   N1 et N2 (1 <= N <= 100 000 000) sont amis si g(N1) == g(N2).
 *   -> "OUI" / "NON"
 *   Exemple 1 : 1234567 -> 28 -> 10 -> 1   et 1425376 -> 28 -> 10 -> 1 : OUI
 *   Exemple 2 : 111111111 -> 9   et 122222222 -> 17 -> 8 : NON
 *
 * IDEE : PIEGE DE L'ENONCE, il ne faut PAS boucler 100 000 fois bêtement
 *   (ça marcherait quand même, mais c'est inutile). Dès que N n'a plus qu'un
 *   chiffre, f(N) = N : la valeur ne bouge plus. Donc
 *   g(N) = on applique f TANT QUE N >= 10.
 *   (Ce résultat s'appelle la "racine numérique".)
 *
 *   Formule directe à connaître : pour N >= 1, g(N) = 1 + (N - 1) % 9.
 *   (Car la somme des chiffres garde le même reste modulo 9.)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - N à un seul chiffre : g(N) = N directement.
 *   - Multiples de 9 : g = 9 (et pas 0 !). 18 -> 9. La formule
 *     N % 9 donnerait 0 : FAUX, d'où 1 + (N-1) % 9.
 *   - N = 0 (hors énoncé) : g = 0, la boucle le gère.
 *   - L'exemple 2 contient 111111111 > 100 000 000 : ça tient dans un int,
 *     mais long long par sécurité.
 *   - Les deux nombres égaux -> OUI.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

/* somme des chiffres de n */
long long f(long long n)
{
    long long somme = 0;
    while (n > 0) {
        somme += n % 10;   /* dernier chiffre */
        n /= 10;           /* on l'enlève     */
    }
    return somme;
}

/* f répétée jusqu'à un seul chiffre (= f répétée 100 000 fois) */
long long g(long long n)
{
    while (n >= 10) {
        n = f(n);
    }
    return n;
}

/* version formule (même résultat, pour vérifier) */
long long g_formule(long long n)
{
    if (n == 0) return 0;
    return 1 + (n - 1) % 9;
}

int main(void)
{
    long long n1, n2;
    scanf("%lld", &n1);
    scanf("%lld", &n2);

    if (g(n1) == g(n2)) printf("OUI\r\n");
    else                printf("NON\r\n");
    return 0;
}
