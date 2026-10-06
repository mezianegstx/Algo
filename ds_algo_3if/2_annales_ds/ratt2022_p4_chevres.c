/*
 * Rattrapage 2022 - Problème 4 : Chèvres (sac à dos 0/1, max <= capacité)
 * =======================================================================
 * ENONCE : capacité n (<= 1200), na chèvres d'Ardèche, nd chèvres de la Drôme,
 *   puis na + nd poids. Seules les chèvres de 50 kg OU MOINS sont autorisées.
 *   Afficher le plus grand poids total <= capacité qu'on peut charger.
 *   Exemple : capacité 80, poids 20 20 60 20 | 30 90 70
 *     autorisées : 20 20 20 30  -> sommes possibles <= 80 : ..., 60, 70
 *     (80 impossible) -> 70
 *
 * IDEE : c'est le sac à dos 0/1 du TD3 (chaque chèvre une seule fois),
 *   mais on ne cherche pas "exactement C" : on cherche le PLUS GRAND s <= C
 *   tel que possible[s] == 1.
 *   1. lire toutes les chèvres, IGNORER celles > 50 kg
 *   2. DP subset sum avec boucle DECROISSANTE
 *   3. parcourir s de C vers 0, le premier possible[s] est la réponse
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Ardèche / Drôme : PIEGE, ça ne change RIEN, c'est juste pour
 *     l'ordre de lecture. Il faut quand même lire na + nd poids.
 *   - "ne dépassent pas 50 kg" -> 50 kg exactement est AUTORISE (<= 50).
 *   - Une chèvre > capacité : la boucle ne fait rien, OK.
 *   - Aucune chèvre autorisée ou toutes trop lourdes -> réponse 0
 *     (possible[0] = 1 toujours).
 *   - Boucle DECROISSANTE (sinon une chèvre serait prise plusieurs fois).
 *   - Le "glouton" (prendre les plus lourdes d'abord) est FAUX :
 *     capacité 100 avec 50, 49, 26, 25 -> glouton 50+49 = 99 (puis plus
 *     rien ne rentre), alors que 49+26+25 = 100. Toujours la DP.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define CMAX 1200
#define POIDS_MAX_ASSURANCE 50

char possible[CMAX + 1];

int main(void)
{
    int C, na, nd, i, s, w;

    scanf("%d", &C);
    scanf("%d", &na);
    scanf("%d", &nd);
    if (C > CMAX) C = CMAX;

    possible[0] = 1;

    for (i = 0; i < na + nd; i++) {          /* Ardèche puis Drôme : pareil */
        scanf("%d", &w);
        if (w > POIDS_MAX_ASSURANCE) continue; /* interdite par l'assurance */
        if (w <= 0 || w > C) continue;
        for (s = C; s >= w; s--) {           /* DECROISSANT : 0/1 */
            if (possible[s - w]) possible[s] = 1;
        }
    }

    /* plus grande somme atteignable <= C */
    int meilleur = 0;
    for (s = C; s >= 0; s--) {
        if (possible[s]) { meilleur = s; break; }
    }

    printf("%d\r\n", meilleur);
    return 0;
}
