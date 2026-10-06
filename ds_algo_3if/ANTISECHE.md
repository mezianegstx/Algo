# Antisèche DS Algorithmique 3IF (domjudge, C)

## 1. Les 6 erreurs qui font perdre des points bêtement

1. **Fin de ligne** : chaque ligne affichée finit par `"\r\n"`, jamais `"\n"` seul.
2. **`return 0;`** à la fin du `main`. Sinon domjudge peut répondre RUN-ERROR.
3. **Rien d'autre que la réponse** : pas de `printf("Entrez n :")`, pas de debug oublié, pas d'espace en trop en fin de ligne.
4. **Format de lecture** : `%d` int, `%lld` long long, `%lf` **double** (avec `%f` la valeur lue est fausse), `%s` mot sans `&`.
5. **Gros tableaux** : plus de ~100 000 cases → en **global**, en `static`, ou `malloc`. En variable locale ça plante (pile).
6. **Débordement** : produit, factorielle, somme de beaucoup de valeurs → `long long`. `(long long)a * b` et pas `a * b`.

## 2. Verdicts domjudge et quoi faire

| Verdict | Cause probable | Réflexe |
|---|---|---|
| CORRECT | | Passer au suivant |
| WRONG-ANSWER | Format (`\r\n`, espaces, majuscules, "OUI" vs "oui"), cas particulier oublié | Relire le format exact de l'exemple, tester 0 / 1 / vide / égalités / négatifs |
| TIMELIMIT | Algo trop lent, ou boucle infinie | Regarder la taille max de n (tableau 4), lecture qui ne s'arrête jamais (EOF, sentinelle) |
| RUN-ERROR | Segfault : indice hors tableau, tableau local trop gros, division par 0, pas de `return 0` | Vérifier les bornes, passer les tableaux en global |
| COMPILER-ERROR | Syntaxe, mauvais langage choisi (C vs C++) | Compiler chez soi d'abord, choisir le bon langage à la soumission |
| NO-OUTPUT | Rien affiché | Oubli du `printf`, ou `return` trop tôt |

Soumettre plusieurs fois n'est pas pénalisé : soumettre dès qu'on a une version qui passe les exemples.

## 3. Quel algo pour quel énoncé ?

| L'énoncé parle de... | Technique | Où regarder |
|---|---|---|
| "remplir exactement", "chaque objet une fois" | Subset sum DP, boucle **décroissante** | `1_td/td3_sac_a_dos_0_1.c` |
| "objets en nombre infini", "autant de fois" | DP, boucle **croissante** | `1_td/td2_2_sac_a_dos_illimite.c` |
| "le maximum sans dépasser la capacité" | Même DP, puis plus grand `s <= C` possible | `2_annales_ds/ratt2022_p4_chevres.c` |
| poids + valeurs à maximiser | Sac à dos valeur | `3_boite_a_outils/05_prog_dynamique.c` |
| "minimum de pièces", "nombre de façons" | DP rendu de monnaie (le glouton est faux) | `05_prog_dynamique.c` |
| "le plus fréquent", "doublons", "médiane", "les 10% plus grands" | **Trier** (qsort) puis parcourir | `ratt2022_p1`, `ds2019_p2` |
| intervalles qui se chevauchent | Trier par début, fusionner | `1_td/td1_6_fusion_intervalles.c` |
| intersection de 2 intervalles | `max(début) .. min(fin)`, 0 si négatif | `1_td/td1_3` |
| lettres, anagrammes, lettres en double | Tableau `compte[26]`, indice `c - 'a'` | `ratt2022_p2`, `ds2019_p3`, `04_chaines.c` |
| somme des chiffres, nombre inversé, palindrome numérique | `% 10` et `/ 10` | `ds2019_p4`, `03_maths.c` |
| diviseurs, premiers, parfaits, "forts" | Boucle jusqu'à `racine(n)`, ou crible | `ratt2022_p3`, `03_maths.c` |
| équations en entiers, petites bornes | Force brute bornée (penser aux négatifs) | `1_td/td2_1_equations.c` |
| distance à un point, cercle | Comparer les **carrés** (`dx²+dy² < R²`) | `ds2019_p1` |
| commandes `insert` / `extract` / max | Tas binaire | `1_td/td4_tas_binaire.c` |
| commandes `queue` / `dequeue`, FIFO | File circulaire | `1_td/td5_file_circulaire.c` |
| clé → valeur, `insert` / `query` / `delete` | Table de hachage | `1_td/td6_table_hachage.c` |
| parenthèses, annuler, dernier entré | Pile | `06_structures.c` |
| grille, labyrinthe, plus court chemin | BFS | `07_recursivite_graphes.c` |
| "tous les sous-ensembles / ordres", n petit | Backtracking | `07_recursivite_graphes.c` |

## 4. Taille de n et complexité acceptable (≈ 1 seconde)

| n max | Complexité OK | Exemple |
|---|---|---|
| 10 | O(n!) | permutations |
| 20 | O(2ⁿ) | sous-ensembles |
| 500 | O(n³) | triple boucle |
| 10 000 | O(n²) | double boucle |
| 1 000 000 | O(n log n) | qsort |
| 10⁸ | O(n) ou mieux | une seule boucle simple, ou une formule |

Sac à dos : O(nb_objets × capacité). 100 × 100 000 = 10⁷, ça passe.

## 5. Pièges du langage C

- `7 / 2` vaut `3` (division entière). Pour 3.5 : `(double)7 / 2`.
- `-7 % 3` vaut `-1` en C. Modulo positif : `((a % m) + m) % m`.
- Ne jamais comparer deux `double` avec `==` après calcul : `fabs(a - b) < 1e-9`.
- `if (x = 5)` affecte au lieu de comparer. Écrire `==`.
- `if (cond);` avec un point-virgule : le bloc s'exécute toujours.
- Chaînes : `strcmp(a, b) == 0` pour comparer, `strcpy` pour copier. Buffer = longueur max **+ 1** (`'\0'`).
- `scanf("%d", &n)` avec `&`, mais `scanf("%s", mot)` sans `&`.
- Après un `scanf("%d")`, un `fgets` lit la fin de ligne restante : faire `getchar()` avant.
- `sqrt`, `fabs`, `pow` : `#include <math.h>` (et `-lm` sous Linux). `pow` renvoie un double, éviter pour des entiers.
- Comparateur `qsort` : ne pas faire `return a - b` (débordement, et faux pour les double).
- Variables locales non initialisées = valeur aléatoire. Les globales valent 0.
- `int t[n]` avec n lu au clavier (VLA) : marche en C99 mais pas sous Visual Studio, et plante si n est grand. Préférer `malloc`.
- Visual Studio refuse `scanf` sans `#define _CRT_SECURE_NO_WARNINGS` en **toute première ligne**.

## 6. Lire l'entrée : les 4 cas

```c
/* n puis n valeurs */
scanf("%d", &n);
for (i = 0; i < n; i++) scanf("%d", &t[i]);

/* jusqu'à une sentinelle (-1 non stocké) */
while (scanf("%d", &x) == 1 && x != -1) t[n++] = x;

/* jusqu'à la fin du fichier */
while (scanf("%d", &x) == 1) { ... }

/* commandes texte */
while (scanf("%63s", cmd) == 1) {
    if (strcmp(cmd, "insert") == 0) { scanf("%d", &v); ... }
    else if (strcmp(cmd, "bye") == 0) break;
}
```

## 7. Afficher un réel comme dans les exemples

`printf("%f")` donne `6.000000`. Les énoncés attendent souvent `6` ou `2.5` :

```c
void afficher_reel(double x) {
    long long e = (long long)x;
    if (x == (double)e) printf("%lld\r\n", e);
    else                printf("%.10g\r\n", x);
}
```
Si l'énoncé impose un nombre de décimales : `printf("%.2f\r\n", x);`

## 8. Tester chez soi avant de soumettre

```
gcc -Wall -o prog prog.c -lm
./prog < test.in              (Linux / Mac / WSL)
prog.exe < test.in            (Windows, invite de commandes)
```
Pour débugger sans gêner le juge : `fprintf(stderr, "x = %d\n", x);` (domjudge ne lit que la sortie standard). Les enlever quand même avant la version finale.

## 9. Stratégie pendant le DS

1. Lire **tous** les sujets (5 min). Repérer le type de chaque problème avec le tableau 3.
2. Commencer par le plus facile. Soumettre dès que les exemples passent.
3. Avant chaque soumission : checklist de `3_boite_a_outils/modele_vide.c`.
4. WRONG-ANSWER : penser aux cas limites (n = 1, valeurs égales, négatifs, "strictement", "inclus", ordre d'affichage, égalités dans un max).
5. Ne pas rester bloqué plus de 20 min sur un sujet : passer au suivant et revenir.
