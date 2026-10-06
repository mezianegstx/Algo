# Entraînement CSES pour le DS d'algo

Ordre classé selon ce qui a le plus de chances de tomber au DS (d'après les annales 2019, 2022 et 2025), avec une difficulté progressive dans chaque thème.

Utilisation : `cses init <ID>` puis `cses test <ID>`, depuis le dossier `CSES/`.

## Priorité 1 : ce qui est tombé aux deux derniers DS

| # | Fait | ID | Problème | Pourquoi |
|---|---|---|---|---|
| 1 | [ ] | [1745](https://cses.fi/problemset/task/1745) | Money Sums | quasiment l'exercice des chèvres (2022 r4) |
| 2 | [ ] | [1635](https://cses.fi/problemset/task/1635) | Coin Combinations I | même mécanique que p4 (2025) |
| 3 | [ ] | [1158](https://cses.fi/problemset/task/1158) | Book Shop | le sac à dos 0/1 classique, à savoir écrire les yeux fermés |
| 4 | [ ] | [1755](https://cses.fi/problemset/task/1755) | Palindrome Reorder | tableau de fréquences, comme les anagrammes (2022 r2) |
| 5 | [ ] | [1713](https://cses.fi/problemset/task/1713) | Counting Divisors | crible des diviseurs, comme les nombres abondants (2022 r3) |

## Priorité 2 : les thèmes présents dans plusieurs DS

| # | Fait | ID | Problème | Pourquoi |
|---|---|---|---|---|
| 6 | [ ] | [1629](https://cses.fi/problemset/task/1629) | Movie Festival | `struct` + `qsort` + glouton (tri, 2019 et 2022) |
| 7 | [ ] | [1090](https://cses.fi/problemset/task/1090) | Ferris Wheel | tri et deux pointeurs, dans la suite de 1640 |
| 8 | [ ] | [1192](https://cses.fi/problemset/task/1192) | Counting Rooms | parcours de grille (2025 p3) |
| 9 | [ ] | [1634](https://cses.fi/problemset/task/1634) | Minimizing Coins | variante de la DP « pièces », pour consolider |
| 10 | [ ] | [2189](https://cses.fi/problemset/task/2189) | Point Location Test | géométrie (2019 ex1, 2025 p2) |

## Priorité 3 : les réflexes utiles

| # | Fait | ID | Problème | Pourquoi |
|---|---|---|---|---|
| 11 | [ ] | [1071](https://cses.fi/problemset/task/1071) | Number Spiral | trouver une formule à partir d'une suite (2025 p1) |
| 12 | [ ] | [1623](https://cses.fi/problemset/task/1623) | Apple Division | masques de bits et sous-ensembles (2019 ex3) |
| 13 | [ ] | [1646](https://cses.fi/problemset/task/1646) | Static Range Sum Queries | sommes préfixes, une technique qui sert partout |
| 14 | [ ] | [1074](https://cses.fi/problemset/task/1074) | Stick Lengths | tri puis médiane (2019 ex2) |
| 15 | [ ] | [1068](https://cses.fi/problemset/task/1068) | Weird Algorithm | les débordements d'`int` et `long long` |

## Priorité 4 : pour aller plus loin

| # | Fait | ID | Problème | Pourquoi |
|---|---|---|---|---|
| 16 | [ ] | [1641](https://cses.fi/problemset/task/1641) | Sum of Three Values | deux pointeurs, version plus difficile |
| 17 | [ ] | [1652](https://cses.fi/problemset/task/1652) | Forest Queries | sommes préfixes en 2D, sur une grille |
| 18 | [ ] | [1638](https://cses.fi/problemset/task/1638) | Grid Paths I | DP sur une grille, qui combine deux thèmes |
| 19 | [ ] | [1193](https://cses.fi/problemset/task/1193) | Labyrinth | BFS et reconstitution du chemin |
| 20 | [ ] | [1643](https://cses.fi/problemset/task/1643) | Maximum Subarray Sum | algorithme de Kadane, un classique |

## Déjà faits

- [x] [1621](https://cses.fi/problemset/task/1621) Distinct Numbers
- [x] [2165](https://cses.fi/problemset/task/2165) Tower of Hanoi
- [x] [1073](https://cses.fi/problemset/task/1073) Towers
- [x] [1633](https://cses.fi/problemset/task/1633) Dice Combinations
- [x] [1640](https://cses.fi/problemset/task/1640) Sum of Two Values

## Conseil pour le DS

Chronomètre-toi sur les problèmes 1 à 5. En examen, c'est la rapidité à reconnaître le type d'algorithme et à écrire son squelette qui fait la différence. Pour chaque nouveau problème, essaie d'identifier sa catégorie avant de commencer à coder.
