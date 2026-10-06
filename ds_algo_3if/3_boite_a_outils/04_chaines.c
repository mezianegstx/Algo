/*
 * BOITE A OUTILS 4 : CHAINES DE CARACTERES
 * ========================================
 * Rappels :
 *   - une chaîne C finit par '\0'. Taille du buffer = longueur max + 1.
 *   - comparer : strcmp(a, b) == 0  (JAMAIS a == b, qui compare des adresses)
 *   - copier   : strcpy(dest, src)   (JAMAIS dest = src)
 *   - longueur : strlen(s)  (O(n) : ne pas l'appeler à chaque tour de boucle,
 *                le stocker dans une variable)
 *   - indice d'une lettre : c - 'a' (0..25), c - 'A', c - '0' pour un chiffre
 *   - ctype.h : isdigit, isalpha, isupper, islower, toupper, tolower, isspace
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Inverser une chaîne en place : "abc" -> "cba" */
void inverser_chaine(char *s)
{
    int i = 0, j = (int)strlen(s) - 1;
    while (i < j) {
        char c = s[i]; s[i] = s[j]; s[j] = c;
        i++; j--;
    }
}

/* Palindrome (ex : "kayak") */
int est_palindrome_chaine(const char *s)
{
    int i = 0, j = (int)strlen(s) - 1;
    while (i < j) {
        if (s[i] != s[j]) return 0;
        i++; j--;
    }
    return 1;
}

/* Palindrome en ignorant espaces/ponctuation et majuscules ("Esope reste ici et se repose") */
int est_palindrome_souple(const char *s)
{
    int i = 0, j = (int)strlen(s) - 1;
    while (i < j) {
        if (!isalnum((unsigned char)s[i])) { i++; continue; }
        if (!isalnum((unsigned char)s[j])) { j--; continue; }
        if (tolower((unsigned char)s[i]) != tolower((unsigned char)s[j])) return 0;
        i++; j--;
    }
    return 1;
}

/* Compter les lettres d'un mot : compte[0] = nb de 'a', ..., compte[25] = 'z' */
void compter_lettres(const char *s, int compte[26])
{
    int i;
    memset(compte, 0, 26 * sizeof(int));
    for (i = 0; s[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)s[i]);
        if (c >= 'a' && c <= 'z') compte[c - 'a']++;
    }
}

/* Anagrammes (mêmes lettres, même nombre de fois) */
int sont_anagrammes(const char *a, const char *b)
{
    int ca[26], cb[26], i;
    compter_lettres(a, ca);
    compter_lettres(b, cb);
    for (i = 0; i < 26; i++) if (ca[i] != cb[i]) return 0;
    return 1;   /* ajouter "&& strcmp(a, b) != 0" si les mots doivent différer */
}

/* Lettre la plus fréquente (à égalité : la première dans l'alphabet) */
char lettre_plus_frequente(const char *s)
{
    int c[26], i, best = 0;
    compter_lettres(s, c);
    for (i = 1; i < 26; i++) if (c[i] > c[best]) best = i;
    return (char)('a' + best);
}

/* Toutes les lettres distinctes ? (pas de lettre en double) */
int lettres_toutes_distinctes(const char *s)
{
    int vu[256] = {0}, i;
    for (i = 0; s[i] != '\0'; i++) {
        unsigned char c = (unsigned char)s[i];
        if (vu[c]) return 0;
        vu[c] = 1;
    }
    return 1;
}

/* Mettre en majuscules */
void en_majuscules(char *s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++) s[i] = (char)toupper((unsigned char)s[i]);
}

/* Compter les voyelles */
int compter_voyelles(const char *s)
{
    int i, n = 0;
    for (i = 0; s[i] != '\0'; i++) {
        if (strchr("aeiouyAEIOUY", s[i]) != NULL && s[i] != '\0') n++;
    }
    return n;
}

/* Nombre d'occurrences d'un motif dans un texte (chevauchements comptés) */
int compter_occurrences(const char *texte, const char *motif)
{
    int n = 0;
    const char *p = texte;
    if (motif[0] == '\0') return 0;
    while ((p = strstr(p, motif)) != NULL) {
        n++;
        p++;            /* p += strlen(motif) pour ne PAS compter les chevauchements */
    }
    return n;
}

/* Chaîne -> entier et entier -> chaîne */
void conversions(void)
{
    char s[] = "1234";
    int n = atoi(s);                       /* "1234" -> 1234 */
    long long m = atoll("123456789012");   /* -> long long   */
    double d = atof("3.14");
    char buf[32];
    sprintf(buf, "%d", n);                 /* 1234 -> "1234" */
    (void)m; (void)d;

    /* chiffre caractère <-> valeur */
    int v = '7' - '0';                     /* '7' -> 7 */
    char c = (char)('0' + 7);              /* 7 -> '7' */
    (void)v; (void)c;
}

/* Découper une ligne en mots (séparateurs : espace, virgule) */
void decouper(char *ligne)
{
    char *mot = strtok(ligne, " ,");
    while (mot != NULL) {
        printf("%s\r\n", mot);
        mot = strtok(NULL, " ,");
    }
}

/* Compression "aaabcc" -> "a3b1c2" */
void compresser(const char *s)
{
    int i = 0, n = (int)strlen(s);
    while (i < n) {
        char c = s[i];
        int k = 0;
        while (i < n && s[i] == c) { k++; i++; }
        printf("%c%d", c, k);
    }
    printf("\r\n");
}

int main(void)
{
    char s[] = "kayak";
    printf("%d %d\r\n", est_palindrome_chaine(s), sont_anagrammes("nectar", "carnet"));
    compresser("aaabcc");
    return 0;
}
