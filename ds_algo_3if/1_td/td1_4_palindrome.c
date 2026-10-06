/*
 * TD1-4 : Palindrome de nombres
 * =============================
 * ENONCE : lire des entiers positifs un par ligne, terminés par -1
 *   (le -1 ne fait pas partie du tableau, max 1000 éléments).
 *   Afficher 1 si le tableau se lit pareil dans les deux sens, 0 sinon.
 *   Exemple : 6 10 7 10 6 -1 -> 1
 *
 * IDEE : deux indices, un au début (i) et un à la fin (j).
 *   Tant que i < j : si t[i] != t[j] -> pas palindrome. Sinon i++, j--.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Tableau vide (juste "-1") : on considère que c'est un palindrome -> 1.
 *   - Un seul élément : palindrome -> 1.
 *   - Taille paire (1 2 2 1) ou impaire (1 2 1) : la condition i < j gère les deux.
 *   - On compare des NOMBRES, pas des chiffres : 12 21 n'est PAS un palindrome
 *     (12 != 21). Si un jour on demande "est-ce que le nombre 12321 est un
 *     palindrome", voir est_nombre_palindrome() plus bas.
 *   - Lecture : on s'arrête au -1 OU à la fin de fichier (scanf != 1).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define MAX 1000

/* BONUS : un nombre (ex 12321) est-il palindrome ? On le retourne et on compare */
int est_nombre_palindrome(long long n)
{
    long long original = n, inverse = 0;
    if (n < 0) return 0;
    while (n > 0) {
        inverse = inverse * 10 + n % 10;  /* on ajoute le dernier chiffre */
        n = n / 10;                       /* on enlève le dernier chiffre */
    }
    return inverse == original;
}

int main(void)
{
    int t[MAX + 1];
    int n = 0, x;

    /* lecture jusqu'au -1 (ou fin de fichier) */
    while (scanf("%d", &x) == 1 && x != -1) {
        if (n <= MAX) t[n++] = x;
    }

    int est_palindrome = 1;   /* on suppose que oui, on cherche un contre-exemple */
    int i = 0, j = n - 1;
    while (i < j) {
        if (t[i] != t[j]) {
            est_palindrome = 0;
            break;
        }
        i++;
        j--;
    }

    printf("%d\r\n", est_palindrome);
    return 0;
}
