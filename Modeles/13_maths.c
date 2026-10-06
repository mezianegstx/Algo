/*
 * 13 - ARITHMÉTIQUE
 *
 * pgcd / ppcm, nombres premiers, cribles, diviseurs, chiffres d'un nombre,
 * changement de base, puissance rapide, modulo, coefficients binomiaux.
 *
 * RAPPELS SUR LES DÉBORDEMENTS :
 *   int       : jusqu'à environ 2.10^9
 *   long long : jusqu'à environ 9.10^18        (printf / scanf : %lld)
 *   - 13! dépasse déjà un int, 21! dépasse un long long.
 *   - un produit de deux int peut déborder AVANT d'être rangé :
 *       long long p = a * b;             FAUX (calculé en int)
 *       long long p = (long long)a * b;  correct
 *   - "donner le résultat modulo 10^9+7" : faire % MOD après CHAQUE
 *     addition et multiplication, pas seulement à la fin.
 */
#include <stdio.h>

#define MOD 1000000007

/* ---- pgcd (Euclide) et ppcm ------------------------------------------- */
long long pgcd(long long a, long long b) {
	while (b != 0) {
		long long r = a % b;
		a = b;
		b = r;
	}
	return a;
}

long long ppcm(long long a, long long b) {
	return a / pgcd(a, b) * b;       /* diviser AVANT de multiplier : évite le débordement */
}

/* ---- un nombre est-il premier ? O(racine de n) ------------------------ */
int est_premier(long long x) {
	if (x < 2) return 0;
	for (long long d = 2; d * d <= x; d++) {     /* d*d <= x : pas besoin de sqrt */
		if (x % d == 0) return 0;
	}
	return 1;
}

/* ---- crible d'Ératosthène : tous les premiers jusqu'à MAXV ------------ */
/* À utiliser quand on teste BEAUCOUP de nombres. Après l'appel,
 * compose[x] == 0  <=>  x est premier (pour x >= 2). */
#define MAXV 1000000
char compose[MAXV + 1];

void crible(int limite) {
	compose[0] = compose[1] = 1;
	for (int p = 2; (long long)p * p <= limite; p++) {
		if (compose[p]) continue;                        /* p n'est pas premier : déjà traité */
		for (int multiple = p * p; multiple <= limite; multiple += p) {
			compose[multiple] = 1;
		}
	}
}

/* ---- diviseurs d'UN nombre : O(racine de n) --------------------------- */
/* Les diviseurs vont par paires (d, x/d) : on ne cherche d que jusqu'à la racine. */
int nb_diviseurs(long long x) {
	int compte = 0;
	for (long long d = 1; d * d <= x; d++) {
		if (x % d == 0) {
			compte++;                        /* d */
			if (d != x / d) compte++;        /* et son partenaire x/d, s'il est différent */
		}
	}
	return compte;
}

long long somme_diviseurs(long long x) {     /* x lui-même compris */
	long long somme = 0;
	for (long long d = 1; d * d <= x; d++) {
		if (x % d == 0) {
			somme += d;
			if (d != x / d) somme += x / d;
		}
	}
	return somme;
}

/* ---- crible des diviseurs : pour TOUS les nombres jusqu'à limite ------ */
/* Même idée que ton DS2022/r3.c (nombres abondants) : au lieu de chercher
 * les diviseurs de chaque nombre, chaque d va "se déposer" sur ses multiples.
 * Après l'appel : somme_div[x] = somme des diviseurs de x (x compris). */
long long somme_div[MAXV + 1];

void crible_diviseurs(int limite) {
	for (int x = 0; x <= limite; x++) somme_div[x] = 0;
	for (int d = 1; d <= limite; d++) {
		for (int multiple = d; multiple <= limite; multiple += d) {
			somme_div[multiple] += d;        /* ADAPTER : ++ pour compter les diviseurs */
		}
	}
}

/* ---- décomposition en facteurs premiers ------------------------------- */
void afficher_facteurs(long long x) {
	for (long long p = 2; p * p <= x; p++) {
		while (x % p == 0) {
			printf("%lld ", p);
			x /= p;
		}
	}
	if (x > 1) printf("%lld ", x);           /* ce qui reste est premier */
	printf("\n");
}

/* ---- chiffres d'un nombre --------------------------------------------- */
/* Schéma : x % 10 = dernier chiffre, x /= 10 = on l'enlève. */
int somme_chiffres(long long x) {
	int somme = 0;
	while (x > 0) {
		somme += x % 10;
		x /= 10;
	}
	return somme;
}

int nb_chiffres(long long x) {
	int compte = 1;
	while (x >= 10) { x /= 10; compte++; }
	return compte;
}

long long renverser(long long x) {           /* 1234 -> 4321 */
	long long r = 0;
	while (x > 0) {
		r = r * 10 + x % 10;
		x /= 10;
	}
	return r;
}

/* ---- changement de base ----------------------------------------------- */
/* Écrit x en base `base` (2 à 16) dans texte[]. */
void vers_base(long long x, int base, char texte[]) {
	const char chiffres[] = "0123456789ABCDEF";
	char envers[70];
	int k = 0;
	if (x == 0) envers[k++] = '0';
	while (x > 0) {
		envers[k++] = chiffres[x % base];    /* on obtient les chiffres de droite à gauche */
		x /= base;
	}
	for (int i = 0; i < k; i++) texte[i] = envers[k - 1 - i];
	texte[k] = '\0';
}

long long depuis_base(const char texte[], int base) {
	long long x = 0;
	for (int i = 0; texte[i] != '\0'; i++) {
		char ch = texte[i];
		int chiffre = (ch >= '0' && ch <= '9') ? ch - '0' : ch - 'A' + 10;
		x = x * base + chiffre;
	}
	return x;
}

/* ---- puissance rapide modulo m : a^e % m en O(log e) ------------------ */
/* Idée : a^e = (a²)^(e/2), fois a si e est impair. */
long long puissance_mod(long long a, long long e, long long m) {
	long long resultat = 1 % m;
	a %= m;
	while (e > 0) {
		if (e % 2 == 1) resultat = resultat * a % m;
		a = a * a % m;
		e /= 2;
	}
	return resultat;
}

/* ---- factorielle modulo MOD ------------------------------------------- */
long long factorielle_mod(int nb) {
	long long f = 1;
	for (int i = 2; i <= nb; i++) f = f * i % MOD;
	return f;
}

/* ---- coefficients binomiaux C(n, k) par le triangle de Pascal --------- */
/* C(n,k) = C(n-1,k-1) + C(n-1,k). Remplit tout le triangle jusqu'à limite. */
#define MAXC 1005
int binome[MAXC][MAXC];

void triangle_pascal(int limite) {
	for (int i = 0; i <= limite; i++) {
		binome[i][0] = 1;
		for (int k = 1; k <= i; k++) {
			binome[i][k] = (binome[i - 1][k - 1] + binome[i - 1][k]) % MOD;
		}
	}
}

/* ---- suites : somme de 1 à n, test "carré parfait" -------------------- */
long long somme_1_a_n(long long nb) {
	return nb * (nb + 1) / 2;
}

int est_carre_parfait(long long x) {
	long long r = 0;
	while (r * r < x) r++;                   /* O(racine de x) ; pour x énorme, voir 02_dichotomie.c */
	return r * r == x;
}

int main(void) {
	printf("pgcd(48,18)=%lld (attendu 6)\n", pgcd(48, 18));
	printf("ppcm(4,6)=%lld (attendu 12)\n", ppcm(4, 6));
	printf("est_premier(97)=%d (attendu 1), (91)=%d (attendu 0)\n", est_premier(97), est_premier(91));

	crible(100);
	int nb_premiers = 0;
	for (int x = 2; x <= 100; x++) if (!compose[x]) nb_premiers++;
	printf("premiers <= 100 : %d (attendu 25)\n", nb_premiers);

	printf("nb_diviseurs(36)=%d (attendu 9)\n", nb_diviseurs(36));
	printf("somme_diviseurs(12)=%lld (attendu 28)\n", somme_diviseurs(12));

	crible_diviseurs(100);
	int abondants = 0;                       /* abondant : somme des diviseurs stricts > x */
	for (int x = 1; x <= 100; x++) if (somme_div[x] - x > x) abondants++;
	printf("abondants <= 100 : %d (attendu 22)\n", abondants);

	printf("facteurs de 360 : ");
	afficher_facteurs(360);                  /* attendu 2 2 2 3 3 5 */

	printf("somme_chiffres(9875)=%d (attendu 29)\n", somme_chiffres(9875));
	printf("nb_chiffres(1000)=%d (attendu 4)\n", nb_chiffres(1000));
	printf("renverser(1234)=%lld (attendu 4321)\n", renverser(1234));

	char texte[70];
	vers_base(13, 2, texte);
	printf("13 en base 2 : %s (attendu 1101)\n", texte);
	vers_base(255, 16, texte);
	printf("255 en base 16 : %s (attendu FF)\n", texte);
	printf("1101 depuis base 2 : %lld (attendu 13)\n", depuis_base("1101", 2));

	printf("2^10 mod 1000 = %lld (attendu 24)\n", puissance_mod(2, 10, 1000));
	printf("3^200 mod MOD = %lld (attendu 136318165)\n", puissance_mod(3, 200, MOD));
	printf("20! mod MOD = %lld (attendu 146326063)\n", factorielle_mod(20));

	triangle_pascal(10);
	printf("C(10,3)=%d (attendu 120)\n", binome[10][3]);
	printf("somme 1..100 = %lld (attendu 5050)\n", somme_1_a_n(100));
	printf("carre parfait 49 : %d (attendu 1), 50 : %d (attendu 0)\n", est_carre_parfait(49), est_carre_parfait(50));
	return 0;
}
