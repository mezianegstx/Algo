# Génère les tests .in/.out avec une solution de référence (ensembles de sommes)
import random, os
random.seed(42)
D = os.path.dirname(os.path.abspath(__file__))

def ref(c, w):
    s = {0}
    for x in w:
        if x <= 50:
            s |= {v + x for v in s if v + x <= c}
    return max(s)

def write(name, c, a, d):
    ent = "\n".join(map(str, [c, len(a), len(d)] + a + d)) + "\n"
    open(f"{D}/{name}.in", "w").write(ent)
    open(f"{D}/{name}.out", "w").write(f"{ref(c, a + d)}\r\n")

write("t01_exemple", 80, [20, 20, 60, 20], [30, 90, 70])
write("t02_poids_50_autorise", 100, [50], [50])
write("t03_poids_51_interdit", 100, [51], [49])
write("t04_toutes_trop_lourdes", 500, [51, 100], [200])
write("t05_plus_lourde_que_camion", 10, [30], [40])
write("t06_remplissage_exact", 100, [25, 25], [50])
write("t07_capacite_1", 1, [1], [2])
write("t08_capacite_max_tout_rentre", 1200, [24] * 50, [24] * 50)
write("t09_capacite_max_depassee", 1200, [50] * 50, [50] * 50)
write("t10_seulement_drome", 60, [100], [30, 30, 7])
write("t11_seulement_ardeche", 60, [30, 30, 7], [100])
write("t12_glouton_piege", 100, [45, 34], [33, 33])
write("t13_petite_capacite_gros_poids", 49, [50, 50], [50])
write("t14_poids_1", 37, [1] * 50, [1] * 50)
write("t15_melange_mix", 1000, [51, 3, 50, 99, 7] * 10, [52, 49, 1000, 2, 48] * 10)
for k in range(1, 11):
    c = random.randint(1, 1200)
    na, nd = random.randint(1, 50), random.randint(1, 50)
    w = lambda: random.choice([random.randint(1, 50), random.randint(1, 50), random.randint(1, 1200)])
    write(f"t{15+k:02d}_aleatoire_{k}", c, [w() for _ in range(na)], [w() for _ in range(nd)])
