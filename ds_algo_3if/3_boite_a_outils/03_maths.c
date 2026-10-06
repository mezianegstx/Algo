/*
 * BOITE A OUTILS 3 : ARITHMETIQUE ET NOMBRES
 * ==========================================
 * PGCD, premiers, diviseurs, chiffres, puissances, bases...
 * Les DS aiment les "nombres spéciaux" (forts, amis, parfaits, palindromes).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* ---------------------------------------------------------------------- */
/* PGCD (Euclide) et PPCM                                                  */
/* ---------------------------------------------------------------------- */
long long pgcd(long long a, long long b)
{
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0) {
        long long r = a % b;
        a = b;
        b = r;
    }
    return a;                  /* pgcd(a, 0) = a */
}

long long ppcm(long long a, long long b)
{
    if (a == 0 || b == 0) return 0;
    return a / pgcd(a, b) * b; /* diviser AVANT de multiplier (débordement) */
}

/* ---------------------------------------------------------------------- */
/* Test de primalité en O(racine(n))                                       */
/* Pièges : 0 et 1 ne sont PAS premiers, 2 est premier.                    */
/* ---------------------------------------------------------------------- */
int est_premier(long long n)
{
    long long d;
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (d = 3; d * d <= n; d += 2) {     /* d*d <= n plutôt que sqrt */
        if (n % d == 0) return 0;
    }
    return 1;
}

/* ---------------------------------------------------------------------- */
/* Crible d'Eratosthène : premier[i] = 1 si i est premier, pour i <= N      */
/* Pour beaucoup de tests de primalité (jusqu'à quelques millions).        */
/* ---------------------------------------------------------------------- */
char *crible(int N)
{
    int i, j;
    char *premier = malloc(N + 1);
    for (i = 0; i <= N; i++) premier[i] = 1;
    premier[0] = 0;
    if (N >= 1) premier[1] = 0;
    for (i = 2; (long long)i * i <= N; i++) {
        if (premier[i]) {
            for (j = i * i; j <= N; j += i) premier[j] = 0;
        }
    }
    return premier;   /* free après usage */
}

/* ---------------------------------------------------------------------- */
/* Somme des diviseurs                                                     */
/*   somme_diviseurs_propres(n) : tous les diviseurs SAUF n (1 inclus)     */
/*   Nombre parfait : somme_diviseurs_propres(n) == n (6, 28, 496)         */
/*   Nombres amicaux (sens classique) : s(a) == b et s(b) == a, a != b     */
/*   Attention au carré parfait : ne pas compter racine(n) deux fois.      */
/* ---------------------------------------------------------------------- */
long long somme_diviseurs_propres(long long n)
{
    long long s = (n > 1) ? 1 : 0, d;
    for (d = 2; d * d <= n; d++) {
        if (n % d == 0) {
            s += d;
            if (d != n / d) s += n / d;
        }
    }
    return s;
}

int nombre_de_diviseurs(long long n)     /* 1 et n compris */
{
    int c = 0;
    long long d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) c += (d * d == n) ? 1 : 2;
    }
    return c;
}

/* Décomposition en facteurs premiers : 360 -> 2 2 2 3 3 5 */
void afficher_facteurs_premiers(long long n)
{
    long long d;
    for (d = 2; d * d <= n; d++) {
        while (n % d == 0) {
            printf("%lld\r\n", d);
            n /= d;
        }
    }
    if (n > 1) printf("%lld\r\n", n);   /* le dernier facteur, s'il reste */
}

/* ---------------------------------------------------------------------- */
/* Chiffres d'un nombre                                                    */
/*   n % 10 = dernier chiffre, n / 10 = on l'enlève                         */
/* ---------------------------------------------------------------------- */
int somme_chiffres(long long n)
{
    int s = 0;
    if (n < 0) n = -n;
    while (n > 0) { s += n % 10; n /= 10; }
    return s;
}

int nombre_chiffres(long long n)          /* 0 a 1 chiffre ! */
{
    int c = 1;
    if (n < 0) n = -n;
    while (n >= 10) { c++; n /= 10; }
    return c;
}

long long inverser_nombre(long long n)    /* 1230 -> 321 */
{
    long long r = 0;
    while (n > 0) { r = r * 10 + n % 10; n /= 10; }
    return r;
}

int est_palindrome_nombre(long long n)    /* 12321 -> 1 */
{
    return n >= 0 && inverser_nombre(n) == n;
}

/* Racine numérique : somme des chiffres répétée jusqu'à 1 chiffre */
int racine_numerique(long long n)
{
    if (n == 0) return 0;
    return 1 + (int)((n - 1) % 9);
}

/* ---------------------------------------------------------------------- */
/* Puissance rapide : a^b en O(log b) (avec modulo optionnel)              */
/* ---------------------------------------------------------------------- */
long long puissance(long long a, int b)
{
    long long r = 1;
    while (b > 0) {
        if (b % 2 == 1) r *= a;
        b /= 2;
        if (b > 0) a *= a;    /* ne pas élever au carré inutilement (débordement) */
    }
    return r;
}

long long puissance_mod(long long a, long long b, long long m)
{
    long long r = 1 % m;
    a %= m;
    if (a < 0) a += m;
    while (b > 0) {
        if (b & 1) r = r * a % m;
        a = a * a % m;
        b >>= 1;
    }
    return r;
}

/* ---------------------------------------------------------------------- */
/* Suites classiques                                                       */
/* ---------------------------------------------------------------------- */
long long fibonacci(int n)     /* F0 = 0, F1 = 1. Itératif (le récursif   */
{                              /* naïf est exponentiel : à éviter)          */
    long long a = 0, b = 1;
    int i;
    for (i = 0; i < n; i++) {
        long long c = a + b;
        a = b;
        b = c;
    }
    return a;
}

long long factorielle(int n)
{
    long long r = 1;
    int i;
    for (i = 2; i <= n; i++) r *= i;
    return r;
}

/* Coefficient binomial C(n, k) sans débordement intermédiaire inutile */
long long binomial(int n, int k)
{
    long long r = 1;
    int i;
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    for (i = 1; i <= k; i++) r = r * (n - k + i) / i;   /* toujours entier */
    return r;
}

/* ---------------------------------------------------------------------- */
/* Conversion de base                                                      */
/* ---------------------------------------------------------------------- */
void afficher_en_base(long long n, int base)    /* n >= 0, base 2..16 */
{
    char chiffres[] = "0123456789ABCDEF";
    char buf[70];
    int k = 0;
    if (n == 0) { printf("0\r\n"); return; }
    while (n > 0) { buf[k++] = chiffres[n % base]; n /= base; }
    while (k > 0) putchar(buf[--k]);          /* à l'envers */
    printf("\r\n");
}

/* ---------------------------------------------------------------------- */
/* Réels : comparer avec une tolérance, arrondir                           */
/* ---------------------------------------------------------------------- */
#define EPS 1e-9
int reels_egaux(double a, double b) { return fabs(a - b) < EPS; }
/* arrondi à l'entier le plus proche : (long long)floor(x + 0.5) ou llround(x) */

/* Modulo toujours positif : en C, -7 % 3 = -1 (et pas 2) ! */
int mod_positif(int a, int m) { return ((a % m) + m) % m; }

int main(void)
{
    printf("%lld %lld %d %lld\r\n", pgcd(12, 18), ppcm(4, 6), est_premier(97), fibonacci(10));
    afficher_en_base(10, 2);
    return 0;
}
