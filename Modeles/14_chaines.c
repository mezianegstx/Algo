/*
 * 14 - CHAÎNES DE CARACTÈRES
 *
 * Une chaîne = un tableau de char terminé par '\0'.
 *   char mot[101];        -> 100 caractères utiles + le '\0'
 *   scanf("%100s", mot);  -> lit UN mot (s'arrête à l'espace)
 *
 * <string.h> :
 *   strlen(s)          longueur (sans le '\0')   -> NE PAS l'appeler dans la
 *                      condition d'une boucle, la calculer une fois avant
 *   strcmp(a, b)       0 si égales, <0 si a avant b, >0 si a après b
 *                      ATTENTION : a == b compare les adresses, pas le texte
 *   strcpy(dest, src)  copie            strcat(dest, src)  ajoute à la fin
 *   strstr(s, motif)   pointeur sur la 1re occurrence de motif, ou NULL
 *   strchr(s, c)       pointeur sur la 1re occurrence du caractère c, ou NULL
 *
 * <ctype.h> :
 *   isdigit(c) isalpha(c) isupper(c) islower(c) isspace(c)
 *   toupper(c) tolower(c)
 *
 * Calculs sur les caractères (ce sont des nombres) :
 *   c - 'a'   rang de la lettre (0..25)      'a' + k   la k-ième lettre
 *   c - '0'   valeur du chiffre (0..9)       '0' + k   le chiffre k
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ---- palindrome ------------------------------------------------------- */
int est_palindrome(const char s[]) {
	int g = 0, d = (int)strlen(s) - 1;
	while (g < d) {
		if (s[g] != s[d]) return 0;
		g++;
		d--;
	}
	return 1;
}

/* ---- inverser sur place ----------------------------------------------- */
void inverser(char s[]) {
	int g = 0, d = (int)strlen(s) - 1;
	while (g < d) {
		char t = s[g]; s[g] = s[d]; s[d] = t;
		g++;
		d--;
	}
}

/* ---- majuscules / minuscules ------------------------------------------ */
void en_majuscules(char s[]) {
	for (int i = 0; s[i] != '\0'; i++) {
		s[i] = (char)toupper((unsigned char)s[i]);
	}
}

/* ---- compter un type de caractère ------------------------------------- */
int nb_voyelles(const char s[]) {
	int compte = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		if (strchr("aeiouyAEIOUY", s[i]) != NULL) compte++;   /* "s[i] est-il dans cette liste ?" */
	}
	return compte;
}

/* ---- compter les mots d'une ligne ------------------------------------- */
/* Un mot commence quand on voit un caractère non-espace juste après un espace. */
int nb_mots(const char ligne[]) {
	int compte = 0, dans_mot = 0;
	for (int i = 0; ligne[i] != '\0'; i++) {
		if (isspace((unsigned char)ligne[i])) {
			dans_mot = 0;
		} else if (!dans_mot) {
			dans_mot = 1;
			compte++;
		}
	}
	return compte;
}

/* ---- nombre d'occurrences d'un motif (chevauchements comptés) --------- */
int nb_occurrences(const char texte[], const char motif[]) {
	int lt = (int)strlen(texte), lm = (int)strlen(motif);
	int compte = 0;
	for (int i = 0; i + lm <= lt; i++) {
		if (strncmp(texte + i, motif, lm) == 0) compte++;     /* compare lm caractères à partir de i */
	}
	return compte;
}

/* ---- plus longue suite de caractères identiques ----------------------- */
/* (CSES Repetitions) Schéma "longueur de la série en cours". */
int plus_longue_repetition(const char s[]) {
	int meilleur = 0, courant = 0;
	for (int i = 0; s[i] != '\0'; i++) {
		if (i > 0 && s[i] == s[i - 1]) courant++;
		else courant = 1;
		if (courant > meilleur) meilleur = courant;
	}
	return meilleur;
}

/* ---- chaîne <-> nombre ------------------------------------------------ */
long long chaine_vers_nombre(const char s[]) {
	long long x = 0;
	int i = 0, signe = 1;
	if (s[0] == '-') { signe = -1; i = 1; }
	for (; s[i] != '\0'; i++) {
		x = x * 10 + (s[i] - '0');
	}
	return signe * x;
}
/* Dans l'autre sens : sprintf(texte, "%lld", x);  (comme printf, mais dans une chaîne) */

/* ---- décalage de lettres (code de César) ------------------------------ */
void cesar(char s[], int decalage) {
	for (int i = 0; s[i] != '\0'; i++) {
		if (s[i] >= 'a' && s[i] <= 'z') {
			s[i] = (char)('a' + (s[i] - 'a' + decalage % 26 + 26) % 26);   /* +26 : marche aussi en négatif */
		}
	}
}

/* ---- découper une ligne selon un séparateur --------------------------- */
/* strtok MODIFIE la chaîne (il remplace les séparateurs par '\0'). */
void afficher_morceaux(char ligne[]) {
	char *morceau = strtok(ligne, " ,;");    /* ADAPTER : liste des séparateurs */
	while (morceau != NULL) {
		printf("[%s] ", morceau);
		morceau = strtok(NULL, " ,;");       /* NULL = "continue sur la même chaîne" */
	}
	printf("\n");
}

int main(void) {
	printf("palindrome kayak : %d (attendu 1), chien : %d (attendu 0)\n", est_palindrome("kayak"), est_palindrome("chien"));

	char s[] = "bonjour";
	inverser(s);
	printf("%s (attendu ruojnob)\n", s);
	en_majuscules(s);
	printf("%s (attendu RUOJNOB)\n", s);

	printf("voyelles : %d (attendu 3)\n", nb_voyelles("bonjour"));
	printf("mots : %d (attendu 4)\n", nb_mots("  le chat   dort bien "));
	printf("occurrences : %d (attendu 3)\n", nb_occurrences("aaaa", "aa"));
	printf("repetition : %d (attendu 3)\n", plus_longue_repetition("ATTCGGGA"));
	printf("nombre : %lld (attendu -1234)\n", chaine_vers_nombre("-1234"));

	char c[] = "xyz";
	cesar(c, 3);
	printf("%s (attendu abc)\n", c);

	char ligne[] = "pomme,poire kiwi;figue";
	afficher_morceaux(ligne);                /* attendu [pomme] [poire] [kiwi] [figue] */

	printf("strcmp(abc, abd) < 0 : %d (attendu 1)\n", strcmp("abc", "abd") < 0);
	printf("'d' - 'a' = %d (attendu 3), '7' - '0' = %d (attendu 7)\n", 'd' - 'a', '7' - '0');

	char assemble[50];
	strcpy(assemble, "bon");
	strcat(assemble, "jour");
	printf("%s, longueur %d (attendu bonjour, 7)\n", assemble, (int)strlen(assemble));
	printf("contient 'jou' : %d (attendu 1)\n", strstr(assemble, "jou") != NULL);
	return 0;
}
