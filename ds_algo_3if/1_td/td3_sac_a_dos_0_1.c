/*
 * TD3 : Sac à dos 2 (chaque objet utilisable UNE SEULE FOIS)
 * ==========================================================
 * ENONCE : capacité C (<= 10000) puis des poids (> 0) terminés par -1
 *   (max 100 objets). Chaque objet au plus une fois.
 *   Peut-on remplir EXACTEMENT le sac ? -> "OUI" / "NON"
 *   Exemple 1 : 24 avec {20, 3, 5, 1} -> OUI (20 + 3 + 1)
 *   Exemple 2 : 24 avec {20, 3, 5}    -> NON
 *
 * IDEE : DP "subset sum" (somme de sous-ensemble).
 *   possible[s] = 1 si on peut faire exactement s avec les objets vus.
 *   possible[0] = 1.
 *   Pour chaque poids w, pour s de C DESCENDANT jusqu'à w :
 *       si possible[s - w] alors possible[s] = 1
 *   Réponse : possible[C].
 *   Complexité : O(nb_objets * C) = 100 * 10000 = 1 million.
 *
 * POURQUOI DESCENDANT ? Si on montait, possible[s - w] pourrait avoir été mis
 *   à 1 PENDANT ce même tour (donc en utilisant déjà w) et on prendrait w
 *   deux fois. En descendant, possible[s - w] vaut encore l'ancienne valeur.
 *   Exemple du piège : C = 6, un seul objet de poids 3.
 *     montant  : possible[3] = 1 puis possible[6] = 1 -> OUI (FAUX)
 *     descendant : possible[6] testé avant possible[3] -> NON (JUSTE)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - C = 0 -> OUI.
 *   - Aucun objet et C > 0 -> NON.
 *   - Poids > C : ignoré (la boucle ne fait rien).
 *   - Objets de même poids (ex : 5, 5) : deux objets distincts, OK de faire 10.
 *   - VARIANTE "plus grande somme <= C" (voir chèvres, rattrapage 2022 P4) :
 *     même DP, puis chercher le plus grand s <= C avec possible[s] == 1.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define CMAX 10000

char possible[CMAX + 1];   /* global = initialisé à 0 */

int main(void)
{
    int C, w, s;
    scanf("%d", &C);

    possible[0] = 1;

    while (scanf("%d", &w) == 1 && w != -1) {
        if (w <= 0 || w > C) continue;
        for (s = C; s >= w; s--) {          /* DECROISSANT = une seule fois */
            if (possible[s - w]) possible[s] = 1;
        }
    }

    if (C >= 0 && C <= CMAX && possible[C]) printf("OUI\r\n");
    else                                    printf("NON\r\n");
    return 0;
}
