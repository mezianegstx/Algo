/*
 * TD2-2 : Sac à dos 1 (objets en quantité ILLIMITEE)
 * ===================================================
 * ENONCE : capacité C (<= 100000) puis des poids (> 0) terminés par -1
 *   (max 100 objets). Chaque objet peut être pris AUTANT DE FOIS qu'on veut.
 *   Peut-on remplir EXACTEMENT le sac ? -> "OUI" / "NON"
 *   Exemple 1 : 24 avec {3, 5, 2} -> OUI     Exemple 2 : 24 avec {20, 5} -> NON
 *
 * IDEE : programmation dynamique (DP) sur les sommes atteignables.
 *   possible[s] = 1 si on peut faire exactement la somme s.
 *   possible[0] = 1 (sac vide).
 *   Pour chaque poids w, pour s de w à C (ordre CROISSANT) :
 *       si possible[s - w] alors possible[s] = 1
 *   Réponse : possible[C].
 *   Complexité : O(nb_objets * C) = 100 * 100000 = 10 millions, OK.
 *
 * !!! LA DIFFERENCE AVEC LE SAC A DOS 0/1 (TD3) TIENT A L'ORDRE DE LA BOUCLE !!!
 *   - ordre CROISSANT (ici) : possible[s - w] a peut-être déjà utilisé w,
 *     donc on peut reprendre w plusieurs fois -> objets ILLIMITES.
 *   - ordre DECROISSANT (TD3) : chaque objet utilisé au plus une fois.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Capacité 0 -> OUI (on ne met rien).
 *   - Aucun objet (juste -1) et C > 0 -> NON.
 *   - Poids > C : il ne sert à rien, la boucle "s de w à C" ne fait rien.
 *   - Tableau de 100001 cases : le déclarer en GLOBAL ou static, pas en
 *     variable locale énorme (risque de dépassement de pile).
 *   - Une version récursive naïve explose en temps : TOUJOURS la DP.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define CMAX 100000

char possible[CMAX + 1];   /* global : initialisé à 0 automatiquement */

int main(void)
{
    int C, w, s;
    scanf("%d", &C);

    possible[0] = 1;       /* somme 0 : toujours faisable */

    while (scanf("%d", &w) == 1 && w != -1) {
        if (w <= 0 || w > C) continue;      /* inutile ou invalide */
        for (s = w; s <= C; s++) {          /* CROISSANT = illimité */
            if (possible[s - w]) possible[s] = 1;
        }
    }

    if (C >= 0 && C <= CMAX && possible[C]) printf("OUI\r\n");
    else                                    printf("NON\r\n");
    return 0;
}
