/*
 * MODELE DE DEPART POUR UN EXERCICE DE DS
 * =======================================
 * Copier ce fichier, remplir, soumettre sur domjudge.
 *
 * CHECKLIST AVANT DE SOUMETTRE :
 *   [ ] chaque printf finit par "\r\n"
 *   [ ] return 0; à la fin du main
 *   [ ] scanf : "%d" int, "%lld" long long, "%lf" double, "%s" mot
 *   [ ] pas de tableau local énorme (> ~100 000 cases) : global, static ou malloc
 *   [ ] débordement ? (produits, sommes, factorielles) -> long long
 *   [ ] testé sur TOUS les exemples de l'énoncé + cas limites (0, 1, vide,
 *       égalités, négatifs, valeur max)
 *   [ ] aucun printf de debug oublié
 */
#define _CRT_SECURE_NO_WARNINGS   /* pour Visual Studio (scanf) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX 100000

int t[MAX + 1];   /* tableau global : initialisé à 0, pas de souci de pile */

int main(void)
{
    int n, i;
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &t[i]);
    }

    /* ... calcul ... */
    int resultat = 0;

    printf("%d\r\n", resultat);
    return 0;
}
