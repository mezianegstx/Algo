/*
 * DS 2019 - Problème 1 : Points proches d'un centre (3 points)
 * ============================================================
 * ENONCE : lire P (x y), le seuil R, n (1 <= n <= 1 000 000) puis n points.
 *   Compter les points dont la distance à P est STRICTEMENT inférieure à R.
 *   Exemple : P = (0, 1), R = 1, points (0,-1) (0,1) (0,0.5) -> 2
 *
 * IDEE : pour chaque point, distance² = dx² + dy². On compare
 *   dx² + dy² < R²   (au lieu de sqrt(dx² + dy²) < R)
 *   C'est équivalent quand R >= 0, plus rapide et sans sqrt.
 *   Pas besoin de stocker les points : on lit et on compte au fur et à mesure.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - STRICTEMENT inférieur : un point pile à distance R ne compte PAS
 *     (le point (0,-1) de l'exemple est à distance 2, le cas "pile" serait (0,0)).
 *   - R <= 0 : aucun point ne peut être à distance < R -> 0. (Avec R² on
 *     compterait à tort si R était négatif, d'où le test.)
 *   - Le point P lui-même dans la liste : distance 0 < R -> compté si R > 0.
 *   - Coordonnées négatives : le carré les rend positives, OK.
 *   - 1 000 000 de points : ne pas les stocker dans un tableau local
 *     (pile !). Ici on ne stocke rien.
 *   - double plutôt que float (l'énoncé propose float, mais double est plus
 *     précis). Lecture "%lf". Si le juge donne un résultat différent d'un
 *     point pile sur la frontière, essayer la version float/sqrt en commentaire.
 *   - Le compteur est un entier, affiché avec "%d".
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void)
{
    double px, py, R, x, y;
    int n, i, compteur = 0;

    scanf("%lf %lf", &px, &py);
    scanf("%lf", &R);
    scanf("%d", &n);

    double R2 = R * R;

    for (i = 0; i < n; i++) {
        scanf("%lf %lf", &x, &y);
        double dx = x - px;
        double dy = y - py;
        if (R > 0 && dx * dx + dy * dy < R2) {
            compteur++;
        }
        /* version "comme l'énoncé" (float + sqrt, ajouter #include <math.h>) :
           if (sqrtf(dx*dx + dy*dy) < R) compteur++;                          */
    }

    printf("%d\r\n", compteur);
    return 0;
}
