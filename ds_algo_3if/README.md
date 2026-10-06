# Dossier DS Algorithmique 3IF (INSA Lyon)

Corrigés en C de tous les TD et annales fournis, commentés pas à pas, avec les cas particuliers expliqués dans chaque fichier. Tous les programmes ont été compilés et vérifiés (exemples des énoncés, cas limites, et des milliers de tests aléatoires comparés à une solution par force brute).

**Commence par lire `ANTISECHE.md`** : pièges, verdicts domjudge, et un tableau "type d'énoncé → technique → fichier".

## Contenu

### `1_td/` : corrigés des TD
| Fichier | Sujet | Technique |
|---|---|---|
| `td1_1_somme_trois_reels.c` | Somme de 3 réels | Affichage propre d'un réel |
| `td1_2_factorielle.c` | n! | Boucle, long long |
| `td1_3_intersection_intervalles.c` | Aire de l'intersection de 2 intervalles | max des débuts, min des fins |
| `td1_4_palindrome.c` | Tableau palindrome | Deux indices i, j |
| `td1_5_collatz.c` | Suite de Collatz | Boucle while |
| `td1_6_fusion_intervalles.c` | Ensemble d'intervalles d'aire minimale | Tri + fusion |
| `td2_1_equations.c` | x²+y²=A et x³+y³=B | Force brute bornée |
| `td2_2_sac_a_dos_illimite.c` | Sac à dos 1 (objets infinis) | DP, boucle croissante |
| `td3_sac_a_dos_0_1.c` | Sac à dos 2 (chaque objet une fois) | DP, boucle décroissante |
| `td4_tas_binaire.c` | Tas binaire max | Percolation haut / bas |
| `td5_file_circulaire.c` | File FIFO circulaire | Indices modulo 100 |
| `td6_table_hachage.c` | Table de hachage, adressage ouvert | Sondage linéaire, état REMOVED |

### `2_annales_ds/` : corrigés des annales
| Fichier | Sujet | Technique |
|---|---|---|
| `ds2019_p1_points_proches.c` | Points à distance < R | Comparer les carrés |
| `ds2019_p2_capteur.c` | Capteur (DS mai 2019) | Tri + moyennes |
| `ds2019_p3_lettres_doubles.c` | Mots à lettres doubles | Compteur de 26 lettres |
| `ds2019_p4_nombres_amis.c` | Nombres amis | Somme des chiffres répétée |
| `ratt2022_p1_plus_frequent.c` | Nombre le plus fréquent | Tri + paquets |
| `ratt2022_p2_anagramme.c` | Test d'anagramme | Compteur de lettres |
| `ratt2022_p3_nombres_forts.c` | Nombres "forts" | Crible des diviseurs |
| `ratt2022_p4_chevres.c` | Chèvres | Sac à dos 0/1, max ≤ capacité |

### `3_boite_a_outils/` : code prêt à copier pour un sujet inconnu
| Fichier | Contenu |
|---|---|
| `modele_vide.c` | Squelette de départ + checklist avant soumission |
| `01_lecture_affichage.c` | Tous les cas de lecture (n valeurs, sentinelle, EOF, lignes, commandes), affichage de réels, matrices |
| `02_tris_recherche.c` | Comparateurs qsort (int, double, chaînes, structures), tris insertion / sélection / fusion / rapide / comptage, dichotomie |
| `03_maths.c` | PGCD, PPCM, premiers, crible, diviseurs, facteurs premiers, chiffres, puissance rapide, Fibonacci, binomial, bases |
| `04_chaines.c` | Palindromes, anagrammes, compteurs de lettres, découpage, conversions, compression |
| `05_prog_dynamique.c` | Toutes les variantes de sac à dos, rendu de monnaie, Kadane, sommes préfixes, LIS, LCS, deux pointeurs |
| `06_structures.c` | Pile (+ parenthèses), liste chaînée, arbre binaire de recherche |
| `07_recursivite_graphes.c` | Sous-ensembles, permutations, Hanoï, BFS sur grille et sur graphe, zones connexes |

### `tests/`
Les exemples des énoncés et des cas limites, avec la sortie attendue exacte (`\r\n` compris).
Pour tout revérifier (Linux, Mac ou WSL), depuis ce dossier : `bash tests/run_tests.sh`

## Choix faits (à connaître)

- **C pur** (pas de C++), compilable sous gcc et Visual Studio (`_CRT_SECURE_NO_WARNINGS` en tête).
- **TD4, TD5, TD6** : l'ossature moodle n'était pas fournie, donc les programmes sont complets (lecture des commandes comprise). Pour le TD6, la fonction de hachage est une djb2 classique ; si tu pars de l'ossature, garde la sienne (les résultats de `stats` ne dépendent pas de la fonction de hachage).
- **TD2-1 (équations)** : les solutions négatives sont incluses (ce sont des entiers). Si le juge n'attend que des naturels, mettre `BORNE_MIN` à 0.
- **Affichage des réels** (TD1-1, TD1-3, TD1-6) : `6` plutôt que `6.000000`, comme dans les exemples. Si le juge veut un nombre fixe de décimales, remplacer par `printf("%.2f\r\n", x)`.
- Les tailles de buffers et de tableaux sont prises plus larges que les bornes des énoncés, par sécurité.
