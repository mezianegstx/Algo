/*
 * Rattrapage 2022 - Problème 2 : Test d'anagramme (4 points)
 * ==========================================================
 * ENONCE : deux mots en minuscules (max 99 caractères).
 *   "o" s'ils sont anagrammes (mêmes lettres, ordre différent), "n" sinon.
 *   nectar / carnet  -> o
 *   exemple / pasbon -> n
 *   azerty / azerty  -> n   (mots IDENTIQUES : pas des anagrammes !)
 *   aaaaaab / baaaaaa -> o  (lettres répétées : il faut compter)
 *
 * IDEE : compter les lettres.
 *   compte[c - 'a']++ pour chaque lettre du mot 1
 *   compte[c - 'a']-- pour chaque lettre du mot 2
 *   Anagrammes <=> toutes les cases valent 0 ET les mots sont différents.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Mots IDENTIQUES -> "n" (exemple 3). Test avec strcmp == 0.
 *   - Longueurs différentes -> forcément "n" (le comptage le détecte aussi).
 *   - Lettres répétées (exemple 4) : vérifier juste "chaque lettre de A est
 *     dans B" ne suffit PAS ("aab" et "abb" donneraient o à tort). Il faut
 *     COMPTER.
 *   - On affiche la lettre "o" (pas le chiffre 0) et "n".
 *   - Buffer : 99 caractères + '\0' = 100 minimum, on prend plus large.
 *   - Les deux mots peuvent être sur la même ligne ou sur deux lignes :
 *     scanf("%s") gère les deux.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

int main(void)
{
    char mot1[256], mot2[256];
    int compte[26];
    int i;

    scanf("%255s", mot1);
    scanf("%255s", mot2);

    /* mots identiques : pas anagrammes */
    if (strcmp(mot1, mot2) == 0) {
        printf("n\r\n");
        return 0;
    }

    memset(compte, 0, sizeof(compte));
    for (i = 0; mot1[i] != '\0'; i++) {
        if (mot1[i] >= 'a' && mot1[i] <= 'z') compte[mot1[i] - 'a']++;
    }
    for (i = 0; mot2[i] != '\0'; i++) {
        if (mot2[i] >= 'a' && mot2[i] <= 'z') compte[mot2[i] - 'a']--;
    }

    int anagramme = (strlen(mot1) == strlen(mot2));
    for (i = 0; i < 26; i++) {
        if (compte[i] != 0) anagramme = 0;
    }

    if (anagramme) printf("o\r\n");
    else           printf("n\r\n");
    return 0;
}
