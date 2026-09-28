# Génère les tests .in/.out avec une solution de référence (dictionnaire de comptage)
import random, os
random.seed(42)
D = os.path.dirname(os.path.abspath(__file__))
IMIN, IMAX = -2**31, 2**31 - 1

def ref(a):
    c = {}
    for x in a:
        c[x] = c.get(x, 0) + 1
    return max(c, key=lambda x: (c[x], x))

def write(name, a):
    open(f"{D}/{name}.in", "w").write("\n".join(map(str, [len(a)] + a)) + "\n")
    open(f"{D}/{name}.out", "w").write(f"{ref(a)}\r\n")

write("t01_exemple1", [1, 2, 3, 4, 5, 2, 2, 3, 4, 5])
write("t02_exemple2", [1, 2, 3, 4, 5, 1, 2, 3, 4, 5])
write("t03_exemple3", [10, 9, 8, 7, 6, 5, 4, 3, 2, 1])
write("t04_un_seul", [7])
write("t05_un_seul_negatif", [-7])
write("t06_deux_differents", [5, 1])
write("t07_deux_egaux", [3, 3])
write("t08_tous_egaux", [4] * 20)
write("t09_egalite_deux_paires", [1, 1, 2, 2])
write("t10_egalite_deux_paires_melange", [2, 1, 2, 1])
write("t11_max_en_tete", [9, 9, 9, 1, 2, 3])
write("t12_max_au_milieu", [1, 5, 5, 5, 8, 9])
write("t13_max_en_fin", [1, 2, 3, 8, 8, 8])
write("t14_petit_plus_frequent", [1, 1, 1, 9, 9])
write("t15_egalite_plus_grand_en_premier", [9, 9, 1, 1])
write("t16_negatifs", [-5, -5, -3, -3, -3, -1])
write("t17_negatifs_egalite", [-5, -5, -3, -3])
write("t18_zero", [0, 0, -1, 1])
write("t19_int_min_seul", [IMIN])
write("t20_int_min_frequent", [IMIN, IMIN, 0, 5])
write("t21_int_min_et_max", [IMIN, IMAX])
write("t22_int_max_frequent", [IMAX, IMAX, IMIN, IMIN, 3])
write("t23_trois_groupes_croissants", [1, 1, 2, 2, 2, 3, 3])
write("t24_dernier_groupe_egal", [1, 1, 1, 2, 3, 3, 3])
write("t25_singletons_puis_paire", [1, 2, 3, 4, 4])
write("t26_paire_puis_singletons", [1, 1, 2, 3, 4])
for k in range(1, 11):
    n = random.randint(1, 50)
    v = random.randint(1, 10)
    write(f"t{26+k:02d}_aleatoire_{k}", [random.randint(-v, v) for _ in range(n)])
write("t37_grand_aleatoire", [random.randint(-1000, 1000) for _ in range(100000)])
write("t38_grand_trie", sorted(random.randint(-1000, 1000) for _ in range(100000)))
write("t39_grand_trie_inverse", sorted((random.randint(-1000, 1000) for _ in range(100000)), reverse=True))
write("t40_grand_tous_egaux", [42] * 100000)
