/*
 * 17 - GÉOMÉTRIE
 *
 * Tombé en 2019 (points dans un disque) et en 2025 (cercles qui se coupent).
 *
 * RÈGLES D'OR :
 *   1. Comparer des distances AU CARRÉ : pas de sqrt, pas d'arrondi.
 *          dist(A,B) < R      <=>     dx*dx + dy*dy < R*R
 *   2. Utiliser double (scanf "%lf") plutôt que float : bien plus précis.
 *      Si les coordonnées sont entières, rester en long long : calcul exact.
 *   3. Ne jamais tester l'égalité de deux double avec == : utiliser une
 *      marge EPS.
 *   4. Lire l'énoncé pour < ou <= ("strictement à l'intérieur" / "sur le
 *      bord compris", "se coupent" / "se touchent").
 *
 * Compilation : ajouter -lm si tu utilises sqrt (gcc 17_geometrie.c -lm).
 */
#include <stdio.h>
#include <math.h>

#define EPS 1e-9

typedef struct {
	double x, y;
} Point;

typedef struct {
	double x, y, r;
} Cercle;

/* ---- distances -------------------------------------------------------- */
double distance_carree(Point a, Point b) {
	double dx = a.x - b.x, dy = a.y - b.y;
	return dx * dx + dy * dy;
}

double distance(Point a, Point b) {          /* seulement si on doit AFFICHER une distance */
	return sqrt(distance_carree(a, b));
}

int distance_manhattan(int x1, int y1, int x2, int y2) {   /* déplacements sur une grille */
	int dx = x1 > x2 ? x1 - x2 : x2 - x1;
	int dy = y1 > y2 ? y1 - y2 : y2 - y1;
	return dx + dy;
}

/* ---- point dans un disque (DS2019 ex1) -------------------------------- */
int dans_disque(Point p, Cercle c) {
	Point centre = {c.x, c.y};
	return distance_carree(p, centre) < c.r * c.r;       /* ADAPTER : <= si le bord compte */
}

/* ---- deux cercles se coupent-ils ? (DS2025 p2) ------------------------ */
/* Les disques se chevauchent si la distance des centres < somme des rayons. */
int disques_se_chevauchent(Cercle a, Cercle b) {
	Point ca = {a.x, a.y}, cb = {b.x, b.y};
	double somme = a.r + b.r;
	return distance_carree(ca, cb) < somme * somme;      /* ADAPTER : <= s'ils peuvent juste se toucher */
}

/* Un disque entièrement à l'intérieur de l'autre : distance + petit rayon <= grand rayon.
 * (À combiner avec le test ci-dessus si l'énoncé parle des CONTOURS qui se croisent.) */
int disque_inclus(Cercle petit, Cercle grand) {
	if (petit.r > grand.r) return 0;
	Point cp = {petit.x, petit.y}, cg = {grand.x, grand.y};
	double marge = grand.r - petit.r;
	return distance_carree(cp, cg) <= marge * marge;
}

/* ---- rectangles alignés sur les axes ---------------------------------- */
/* Rectangle donné par deux coins opposés (x1,y1) bas-gauche, (x2,y2) haut-droit. */
int dans_rectangle(double px, double py, double x1, double y1, double x2, double y2) {
	return px >= x1 && px <= x2 && py >= y1 && py <= y2;
}

/* Aire commune à deux rectangles : c'est l'intersection des intervalles en
 * x multipliée par celle en y (même formule que pour deux intervalles). */
double aire_intersection(double ax1, double ay1, double ax2, double ay2,
                         double bx1, double by1, double bx2, double by2) {
	double largeur = fmin(ax2, bx2) - fmax(ax1, bx1);
	double hauteur = fmin(ay2, by2) - fmax(ay1, by1);
	if (largeur <= 0 || hauteur <= 0) return 0;
	return largeur * hauteur;
}

/* ---- produit vectoriel : de quel côté est un point ? ------------------ */
/* Signe de (B - A) x (C - A), en entiers donc EXACT :
 *     > 0 : C est à GAUCHE de la droite A -> B  (on tourne à gauche)
 *     < 0 : C est à DROITE
 *     = 0 : A, B, C sont alignés
 * (CSES Point Location Test) */
long long produit_vectoriel(long long ax, long long ay, long long bx, long long by, long long cx, long long cy) {
	return (bx - ax) * (cy - ay) - (by - ay) * (cx - ax);
}

/* ---- aire d'un polygone (formule du lacet) ---------------------------- */
/* Sommets donnés dans l'ordre du contour. Renvoie 2 x l'aire (reste entier). */
long long double_aire(const long long x[], const long long y[], int n) {
	long long somme = 0;
	for (int i = 0; i < n; i++) {
		int j = (i + 1) % n;                             /* sommet suivant, en bouclant */
		somme += x[i] * y[j] - x[j] * y[i];
	}
	return somme < 0 ? -somme : somme;
}

/* ---- comparer deux réels ---------------------------------------------- */
int presque_egaux(double a, double b) {
	return fabs(a - b) < EPS;
}

int main(void) {
	Point a = {0, 0}, b = {3, 4};
	printf("distance=%.2f (attendu 5.00)\n", distance(a, b));
	printf("manhattan=%d (attendu 7)\n", distance_manhattan(0, 0, 3, 4));

	Cercle c = {0, 0, 5};
	Point dedans = {3, 3}, bord = {3, 4};
	printf("dans disque : %d (attendu 1), sur le bord : %d (attendu 0 car strict)\n", dans_disque(dedans, c), dans_disque(bord, c));

	Cercle c1 = {0, 0, 2}, c2 = {3, 0, 2}, c3 = {10, 0, 1}, c4 = {0.5, 0, 1};
	printf("chevauchement proches : %d (attendu 1), eloignes : %d (attendu 0)\n",
	       disques_se_chevauchent(c1, c2), disques_se_chevauchent(c1, c3));
	printf("inclus : %d (attendu 1)\n", disque_inclus(c4, c1));

	printf("dans rectangle : %d (attendu 1)\n", dans_rectangle(2, 2, 0, 0, 4, 3));
	printf("aire commune=%.1f (attendu 1.0)\n", aire_intersection(0, 0, 3, 2, 2, 1, 5, 5));

	printf("gauche : %d (attendu 1), droite : %d (attendu 1), alignes : %d (attendu 1)\n",
	       produit_vectoriel(0, 0, 2, 0, 1, 1) > 0, produit_vectoriel(0, 0, 2, 0, 1, -1) < 0,
	       produit_vectoriel(0, 0, 2, 2, 5, 5) == 0);

	long long px[] = {0, 4, 4, 0}, py[] = {0, 0, 3, 3};
	printf("aire=%.1f (attendu 12.0)\n", double_aire(px, py, 4) / 2.0);

	printf("0.1+0.2 == 0.3 ? avec == : %d, avec EPS : %d (attendu 0 puis 1)\n", 0.1 + 0.2 == 0.3, presque_egaux(0.1 + 0.2, 0.3));
	return 0;
}
