/*
 * DS 2019 - Problème 3 : Mots contenant des lettres doubles (5 points)
 * ====================================================================
 * ENONCE : n puis n mots (majuscules A-Z, taille < 100). Compter les mots
 *   où au moins une lettre apparaît 2 fois ou plus (pas forcément côte à côte).
 *   Exemple : LANGAGE (A, G), ALGORITHME (aucune), PROGRAMMATION (R, A, M, O)
 *   -> 2
 *
 * IDEE : pour chaque mot, un tableau compte[26] remis à 0.
 *   Pour chaque lettre c : compte[c - 'A']++. Si ça dépasse 1 -> le mot a une
 *   lettre double, on le compte UNE fois et on passe au mot suivant.
 *   ('A' - 'A' = 0, 'B' - 'A' = 1, ..., 'Z' - 'A' = 25)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Plusieurs lettres doubles ou une lettre triple : le mot compte UNE fois
 *     (le break évite de compter plusieurs fois).
 *   - Remettre compte[] à 0 POUR CHAQUE MOT (sinon les mots se mélangent).
 *   - Mot d'une seule lettre : jamais de double.
 *   - Lettres pas forcément consécutives : "ABA" compte.
 *   - Sécurité : si une minuscule apparaissait, on la convertit (toupper).
 *     Tout caractère hors A-Z est ignoré (pas de sortie de tableau).
 *   - Buffer de 101 minimum (100 caractères + '\0'). On prend plus large.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* renvoie 1 si le mot contient au moins une lettre en double */
int a_lettre_double(const char *mot)
{
    int compte[26];
    int i;
    memset(compte, 0, sizeof(compte));   /* remise à 0 pour CE mot */

    for (i = 0; mot[i] != '\0'; i++) {
        char c = (char)toupper((unsigned char)mot[i]);
        if (c < 'A' || c > 'Z') continue;          /* sécurité */
        compte[c - 'A']++;
        if (compte[c - 'A'] >= 2) return 1;        /* trouvé, on arrête */
    }
    return 0;
}

int main(void)
{
    int n, i, resultat = 0;
    char mot[1024];

    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        if (scanf("%1023s", mot) != 1) break;
        if (a_lettre_double(mot)) resultat++;
    }

    printf("%d\r\n", resultat);
    return 0;
}
