# Modèles d'algorithmes pour le DS

Chaque fichier `.c` est autonome : il compile seul, et son `main` lance des petits exemples dont le résultat attendu est affiché à côté. Les fonctions sont faites pour être copiées dans ta solution.

```bash
gcc -Wall -o test 06_dp_sac_a_dos.c -lm && ./test
```

Dans les fichiers, cherche le mot **`ADAPTER`** : il marque les lignes à changer selon l'énoncé (sens d'une comparaison, caractère « mur », taille maximale, etc.).

## 1. Quel fichier pour quel énoncé ?

| Ce que dit l'énoncé | Technique | Fichier |
|---|---|---|
| Format d'entrée inhabituel (sentinelle `-1`, lettre + nombre, grille, fin de fichier) | lecture | `00_lecture_entrees.c` |
| « trier », « classer », « les k plus grands », plusieurs critères | `qsort` + `cmp` | `01_qsort_comparaisons.c` |
| Tableau trié + beaucoup de recherches ; « le plus petit X tel que… » | dichotomie | `02_dichotomie.c` |
| « deux valeurs dont la somme vaut… », « plus long morceau consécutif tel que… » | deux pointeurs, fenêtre | `03_deux_pointeurs_fenetre.c` |
| Beaucoup de questions « somme de la case l à la case r » | sommes préfixes | `04_sommes_prefixes.c` |
| Anagrammes, lettre la plus fréquente, valeurs distinctes, doublons | tableau de fréquences | `05_frequences_comptage.c` |
| « peut-on atteindre exactement… », pièces, poids, capacité d'un camion | DP sac à dos | `06_dp_sac_a_dos.c` |
| « plus longue sous-suite », « somme maximale », comparer deux mots | DP sur séquences | `07_dp_sequences.c` |
| Grille, déplacements seulement droite / bas, compter ou optimiser | DP sur grille | `08_dp_grille.c` |
| « toutes les façons », « tous les arrangements », n ≤ 20 | backtracking | `09_backtracking.c` |
| Ensemble de lettres vues, tous les sous-ensembles | masques de bits | `10_masques_bits.c` |
| Grille : zones, pièces, labyrinthe, plus court chemin, pions alignés | flood fill, BFS | `11_grille_parcours.c` |
| Villes et routes, chefs et employés, tâches avec prérequis | graphes | `12_graphes.c` |
| Diviseurs, nombres premiers, pgcd, chiffres, « modulo 10⁹+7 » | arithmétique | `13_maths.c` |
| Mots, palindromes, compter des caractères | chaînes | `14_chaines.c` |
| Intervalles, horaires, réservations, « un maximum de… » | tri + glouton | `15_intervalles_glouton.c` |
| Parenthèses, « le plus petit à chaque étape » | pile, file, tas | `16_structures.c` |
| Points, cercles, rectangles, distances | géométrie | `17_geometrie.c` |
| `qsort` interdit, ou « nombre d'inversions » | tris à la main | `18_tris_a_la_main.c` |

### Choisir d'après la taille des données

Un ordinateur fait environ 10⁸ opérations simples par seconde. La taille maximale de `n` dans l'énoncé te dit donc quelle complexité est acceptable :

| n maximum | Complexité visée | Techniques possibles |
|---|---|---|
| ≤ 10 | n! | permutations (backtracking) |
| ≤ 20 | 2ⁿ | sous-ensembles (backtracking, masques) |
| ≤ 500 | n³ | trois boucles |
| ≤ 5 000 | n² | deux boucles, DP à deux dimensions |
| ≤ 200 000 | n log n | tri, dichotomie, tas |
| ≤ 10⁷ | n | un seul passage, préfixes, deux pointeurs |
| ≥ 10⁹ | log n ou formule | dichotomie sur la réponse, maths |

## 2. Les techniques que tu n'as pas encore vues

### Sommes préfixes (`04`)

On te pose beaucoup de questions du type « somme des cases l à r ». Refaire une boucle à chaque question est trop lent. On calcule une seule fois `pref[i]` = somme des `i` premières cases ; ensuite toute somme d'intervalle est une soustraction :

```
somme(l..r) = pref[r+1] - pref[l]
```

Le tableau `pref` a une case de plus que le tableau d'origine et `pref[0] = 0`, ce qui évite tous les cas particuliers. La même idée marche en 2D pour la somme d'un rectangle dans une grille. Pour *compter* des cases (« combien d'arbres entre l et r ? »), on fait les préfixes d'un tableau de 0 et de 1.

### Dichotomie sur la réponse (`02`)

Tu connais la dichotomie dans un tableau trié (Towers). La même mécanique sert quand **on ne sait pas calculer la réponse, mais on sait vérifier une réponse proposée**.

Exemple : « temps minimal pour fabriquer 7 objets avec ces machines ». On ne sait pas le calculer directement, mais pour un temps `t` donné, on sait dire si c'est suffisant. Et si `t` suffit, tout temps plus long suffit aussi. On cherche donc par dichotomie la frontière entre « insuffisant » et « suffisant ».

Recette : écrire `int possible(x)`, choisir une borne basse et une borne haute, puis recopier la boucle de `temps_minimal`.

### Fenêtre glissante (`03`)

Pour les questions sur des morceaux **consécutifs** d'un tableau (« plus long morceau de somme ≤ S »). On tient une fenêtre `[g, d]` : `d` avance d'une case à chaque tour, et on avance `g` tant que la fenêtre ne respecte plus la condition. Les deux indices ne reculent jamais, donc c'est en O(n) au lieu de O(n²). Cela suppose que rétrécir la fenêtre « répare » la condition, ce qui est vrai avec des valeurs positives.

### Programmation dynamique (`06`, `07`, `08`)

Tu en fais déjà : ton sac à dos avec le tableau `reachable[]` est une DP. L'idée générale est de **résoudre le problème pour toutes les petites tailles et de réutiliser ces résultats** au lieu de les recalculer.

Méthode en quatre questions :

1. Que représente `dp[i]` ? Il faut pouvoir l'écrire en une phrase.
2. Comment obtenir `dp[i]` à partir de cases déjà calculées ?
3. Quels sont les cas de base (`dp[0]`) ?
4. Où lire la réponse finale ?

**Famille sac à dos (`06`).** Il y a trois décisions, expliquées en tête du fichier :

| Décision | Choix | Conséquence dans le code |
|---|---|---|
| Que contient `dp[c]` ? | vrai/faux, nombre de façons, minimum, maximum | `=1`, `+=`, `min`, `max` |
| Objet utilisable… | une seule fois | boucle sur `c` **descendante** |
| | à volonté | boucle sur `c` **montante** |
| Si on compte des façons : | l'ordre compte (2+3 ≠ 3+2) | boucle capacité **dehors** |
| | l'ordre ne compte pas | boucle objets **dehors** |

Le sac à dos « avec valeurs » (`valeur_max_une_fois`) est celui que tu n'as pas encore écrit : chaque objet a un poids et une valeur, et `dp[c]` vaut la meilleure valeur totale pour un poids ≤ `c`.

**Mémoïsation.** C'est la même chose écrite en récursif : on écrit la fonction récursive naïve et on range chaque résultat dans un tableau pour ne pas le recalculer (`nb_escaliers` dans `07`).

**Kadane (`07`).** Somme maximale d'un morceau consécutif, en un seul passage. On garde « la meilleure somme qui se termine ici » : si elle devient négative, on repart de zéro, car un passé négatif ne peut que pénaliser la suite.

**Plus longue sous-suite croissante, LIS (`07`).** Une sous-suite garde des cases dans l'ordre, pas forcément voisines. Version simple en O(n²) : `dp[i]` = longueur de la meilleure sous-suite qui finit en `i`, on regarde tous les `j < i` plus petits. La version rapide est exactement ton Towers.

**LCS et distance d'édition (`07`).** Deux mots, tableau `dp[i][j]` sur les `i` premières lettres de l'un et les `j` premières de l'autre. Si les deux lettres sont égales, on vient de la diagonale ; sinon on prend le meilleur entre « au-dessus » et « à gauche ».

**DP sur grille (`08`).** Quand on ne peut aller que vers la droite ou le bas, une case ne peut venir que du haut ou de la gauche. On additionne les deux pour compter les chemins, on prend le meilleur des deux pour optimiser.

### Backtracking (`09`)

C'est « essayer toutes les possibilités » de façon organisée. On construit une solution choix par choix ; quand on a exploré tout ce qui découle d'un choix, on le **défait** et on essaie le suivant :

```c
void explorer(int etape) {
    if (etape == FIN) { /* solution complète */ return; }
    for (chaque choix possible) {
        if (!autorise(choix)) continue;   // élagage
        faire(choix);
        explorer(etape + 1);
        defaire(choix);
    }
}
```

Trois formes reviennent tout le temps, et elles sont dans le fichier :

- **sous-ensembles** : pour chaque élément, deux branches (je le prends / je ne le prends pas) ;
- **permutations** : à chaque position, essayer tous les éléments pas encore utilisés (tableau `utilise[]`) ;
- **combinaisons** : comme les permutations, mais en choisissant toujours en ordre croissant.

Le coût est exponentiel : à réserver aux petits `n` (voir le tableau des tailles). L'**élagage** consiste à abandonner une branche dès qu'on sait qu'elle ne mènera à rien (par exemple quand la somme dépasse déjà la cible).

Les masques de bits (`10`) font la même énumération de sous-ensembles avec une simple boucle de `0` à `2ⁿ - 1`.

### Parcours de grille (`11`)

**Les tableaux de directions.** Plutôt que d'écrire quatre fois le même code pour haut, bas, gauche, droite :

```c
const int dl[4] = {-1, 1, 0, 0};
const int dc[4] = {0, 0, -1, 1};
for (int d = 0; d < 4; d++) { int l2 = l + dl[d], c2 = c + dc[d]; ... }
```

**Remplissage (flood fill).** Partir d'une case et marquer, de proche en proche, toutes les cases libres qu'on peut atteindre. Pour compter les zones : parcourir la grille, et à chaque case libre pas encore marquée, lancer un remplissage et ajouter 1.

**Parcours en largeur (BFS).** Donne le plus court chemin en nombre de pas. On utilise une **file** : on traite les cases dans l'ordre où on les a découvertes, donc d'abord celles à distance 1, puis 2, puis 3… La première fois qu'on atteint une case, c'est forcément par un plus court chemin. Pour retrouver le chemin lui-même, on note pour chaque case par quelle direction on y est arrivé, puis on remonte depuis l'arrivée.

**Alignements.** `a_aligne` teste k pions à la suite dans les 4 directions. C'est le sujet DS2025 p3 : le modèle redonne les trois réponses des exemples fournis.

### Graphes (`12`)

Une grille est un cas particulier de graphe. Dans le cas général, on stocke pour chaque sommet la liste de ses voisins (liste d'adjacence, expliquée en tête du fichier). Ensuite :

- **DFS** (en profondeur, récursif) : composantes connexes, calculs dans un arbre « en remontant » (taille des sous-arbres) ;
- **BFS** : plus court chemin quand toutes les arêtes valent 1 ;
- **Dijkstra** : plus court chemin quand les arêtes ont des longueurs différentes (positives) ;
- **tri topologique** : mettre dans un ordre valide des tâches qui ont des prérequis ;
- **union-find** : fusionner des groupes et demander « ces deux-là sont-ils dans le même groupe ? ».

C'est la partie la moins probable au DS d'après les annales ; garde-la surtout pour la taille des sous-arbres et les composantes.

### Glouton (`15`)

Trier selon le bon critère, puis faire un seul passage en prenant à chaque fois le choix évident. La difficulté est de trouver le critère :

- fusionner des intervalles → trier par **début** ;
- caser un maximum d'activités → trier par **fin** (celle qui finit le plus tôt laisse le plus de place) ;
- compter combien sont ouverts en même temps → transformer chaque intervalle en deux événements `+1` / `-1` et trier les événements (ce que tu as fait dans Intervalles2).

Un glouton n'est pas toujours correct. Exemple : rendre 10 avec des pièces 1, 5, 7, le glouton prend 7+1+1+1 alors que 5+5 est meilleur. En cas de doute, cherche un petit contre-exemple à la main ; si tu en trouves un, c'est une DP.

### Pile et tas (`16`)

Tu connais déjà les deux structures. Le fichier ajoute des usages typiques : la pile pour vérifier des parenthèses et pour trouver « le plus proche plus petit à gauche », le tas pour « prendre le plus petit, en remettre un nouveau, recommencer ».

### Produit vectoriel (`17`)

Pour trois points A, B, C, le signe de `(B−A) × (C−A)` dit si C est à gauche, à droite ou sur la droite AB. Tout se fait en entiers, donc sans erreur d'arrondi. Sert pour « de quel côté », « points alignés », et l'aire d'un polygone.

## 3. Pièges repérés dans tes fichiers

À relire avant le DS : ce sont des erreurs que tu as déjà faites.

| Fichier | Problème | Correction |
|---|---|---|
| `Palindromes.c` | `malloc(size)` alloue 4 octets, pas 4 entiers | `malloc(size * sizeof(int))` |
| `DS2019/ex1.c` | `for (int i; i<n; i++)` : `i` n'est pas initialisé | `int i = 0` |
| `SacADosIt.c` | `for (i = 0; i <= size; i++)` lit une case après la fin | `i < size` |
| `DS2022/r2.c` | la 2ᵉ boucle décompte `mot1` au lieu de `mot2` ; compteurs en `char` | `mot2[i++]`, tableau d'`int` |
| `QuickSort.c` | trie mal environ 1 tableau aléatoire sur 10 (ex. `5 3 4 2 4 5 3`) : le pivot est suivi par son indice, or il se déplace pendant les échanges | copier la valeur du pivot, voir `18_tris_a_la_main.c` |
| `CSES/1674-Subordinates` | incrémente tous les indices inférieurs au chef, ce qui ne compte pas les subordonnés | `calculer_tailles` dans `12_graphes.c` |
| `DS2025/p2.c` | `malloc` dans la boucle jamais libéré, `float` et `sqrt` | une variable locale `Circle c`, `double`, distances au carré (`17`) |

Pièges généraux :

- **Débordement** : une somme ou un produit de grands `int` dépasse 2·10⁹. Utiliser `long long` et `%lld`. `(long long)a * b`, pas `a * b`.
- **`" %c"`** : toujours un espace avant `%c` dans `scanf`, sinon on lit le retour à la ligne précédent.
- **Gros tableaux** : les déclarer en global (hors de `main`), sinon la pile déborde autour d'un million de cases.
- **Priorité des opérateurs de bits** : `&`, `|` et `^` passent après `==`, donc `m & bit == 0` se lit `m & (bit == 0)`. Toujours écrire `(m & bit) == 0`. Pour les décalages, mettre aussi les parenthèses : `1 << (c - 'A')`.
- **Division entière** : `a / b` entre deux `int` tronque. Écrire `(double)a / b` ou `a / 2.0`.
- **`qsort`** : jamais `return x - y` dans un `cmp`, cela déborde.
- **Fin de ligne** : tes fichiers utilisent tantôt `\r\n`, tantôt `\n`. Les modèles affichent `\n` ; vérifie ce qu'attend le correcteur.
