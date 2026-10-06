/*
 * TD2-1 : Equations x² + y² = A et x³ + y³ = B
 * =============================================
 * ENONCE : lire A puis B (1 <= A, B <= 50000). Afficher toutes les solutions
 *   entières (x, y), une par ligne "x y", par x croissant.
 *   Une solution avec x = y est affichée DEUX fois. Aucune solution -> "-".
 *   Exemple 1 : 13 / 35 -> "2 3" puis "3 2"
 *   Exemple 2 : 45 / 98 -> "-"
 *
 * IDEE : force brute bornée.
 *   x² <= A <= 50000 donc |x| <= 223 (223² = 49729, 224² = 50176).
 *   Pareil pour y. On teste tous les couples x, y dans [-224 ; 224] :
 *   449 * 449 ≈ 200 000 tests, c'est instantané.
 *   En bouclant x de -224 à 224 on obtient directement l'ordre croissant de x.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - Les entiers peuvent être NEGATIFS (un cube négatif est négatif).
 *     Ex : A = 2, B = 0 -> (-1, 1) et (1, -1). Si le juge n'attend que des
 *     naturels, remplacer BORNE_MIN par 0.
 *   - x = y : la double boucle ne la trouve qu'UNE fois, or l'énoncé veut
 *     l'afficher deux fois -> on l'affiche deux fois explicitement.
 *     Ex : A = 2, B = 2 -> "1 1" puis "1 1".
 *   - Pour un même x il ne peut y avoir qu'un seul y (le signe de y est fixé
 *     par la 2e équation), donc l'ordre par x suffit.
 *   - Aucune solution -> afficher "-" (avec \r\n).
 *   - Cubes : 224³ ≈ 11 millions, tient dans un int. long long par sécurité.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define BORNE_MIN (-224)
#define BORNE_MAX 224

int main(void)
{
    long long A, B;
    long long x, y;
    int trouve = 0;

    scanf("%lld", &A);
    scanf("%lld", &B);

    for (x = BORNE_MIN; x <= BORNE_MAX; x++) {
        for (y = BORNE_MIN; y <= BORNE_MAX; y++) {
            if (x * x + y * y == A && x * x * x + y * y * y == B) {
                printf("%lld %lld\r\n", x, y);
                if (x == y) {
                    printf("%lld %lld\r\n", x, y);   /* x = y : affiché 2 fois */
                }
                trouve = 1;
            }
        }
    }

    if (!trouve) {
        printf("-\r\n");
    }
    return 0;
}
