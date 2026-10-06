/*
 * BOITE A OUTILS 6 : STRUCTURES DE DONNEES
 * ========================================
 * Pile, liste chaînée, arbre binaire de recherche.
 * La file circulaire est dans 1_td/td5, le tas binaire dans 1_td/td4,
 * la table de hachage dans 1_td/td6.
 *
 *   structure        ajout        retrait        recherche
 *   pile (LIFO)      O(1) haut    O(1) haut      -
 *   file (FIFO)      O(1) fin     O(1) début     -
 *   tas max          O(log n)     O(log n) max   max en O(1)
 *   table hachage    O(1) moyen   O(1) moyen     O(1) moyen
 *   ABR équilibré    O(log n)     O(log n)       O(log n) (O(n) si dégénéré)
 *   liste chaînée    O(1) tête    O(n)           O(n)
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ====================================================================== */
/* PILE (LIFO) dans un tableau qui grandit                                 */
/* Utile : parenthèses équilibrées, inverser, évaluer une expression.      */
/* ====================================================================== */
typedef struct {
    int *t;
    int sommet;      /* nombre d'éléments */
    int capacite;
} Pile;

void pile_init(Pile *p)        { p->capacite = 16; p->sommet = 0; p->t = malloc(16 * sizeof(int)); }
int  pile_vide(Pile *p)        { return p->sommet == 0; }
void pile_liberer(Pile *p)     { free(p->t); }

void empiler(Pile *p, int v)
{
    if (p->sommet == p->capacite) {
        p->capacite *= 2;
        p->t = realloc(p->t, p->capacite * sizeof(int));
    }
    p->t[p->sommet++] = v;
}

int depiler(Pile *p)           /* vérifier pile_vide AVANT ! */
{
    return p->t[--p->sommet];
}

int sommet(Pile *p)            /* regarder sans retirer */
{
    return p->t[p->sommet - 1];
}

/* Exemple : parenthèses bien formées "([]{})" -> 1, "(]" -> 0 */
int parentheses_ok(const char *s)
{
    Pile p;
    int i, ok = 1;
    pile_init(&p);
    for (i = 0; s[i] && ok; i++) {
        char c = s[i];
        if (c == '(' || c == '[' || c == '{') {
            empiler(&p, c);
        } else if (c == ')' || c == ']' || c == '}') {
            if (pile_vide(&p)) { ok = 0; break; }       /* fermante en trop */
            char o = (char)depiler(&p);
            if ((c == ')' && o != '(') || (c == ']' && o != '[') || (c == '}' && o != '{'))
                ok = 0;
        }
    }
    if (!pile_vide(&p)) ok = 0;                          /* ouvrante en trop */
    pile_liberer(&p);
    return ok;
}

/* ====================================================================== */
/* LISTE CHAINEE simple                                                    */
/* Astuce : passer Noeud **tete pour pouvoir modifier la tête.             */
/* ====================================================================== */
typedef struct Noeud {
    int val;
    struct Noeud *suiv;
} Noeud;

Noeud *nouveau_noeud(int v)
{
    Noeud *n = malloc(sizeof(Noeud));
    n->val = v;
    n->suiv = NULL;
    return n;
}

void ajouter_tete(Noeud **tete, int v)
{
    Noeud *n = nouveau_noeud(v);
    n->suiv = *tete;
    *tete = n;
}

void ajouter_fin(Noeud **tete, int v)
{
    Noeud *n = nouveau_noeud(v);
    if (*tete == NULL) { *tete = n; return; }          /* liste vide */
    Noeud *c = *tete;
    while (c->suiv != NULL) c = c->suiv;
    c->suiv = n;
}

void inserer_trie(Noeud **tete, int v)                /* garde la liste triée */
{
    Noeud **c = tete;
    while (*c != NULL && (*c)->val < v) c = &(*c)->suiv;
    Noeud *n = nouveau_noeud(v);
    n->suiv = *c;
    *c = n;
}

int supprimer_valeur(Noeud **tete, int v)             /* 1re occurrence */
{
    Noeud **c = tete;
    while (*c != NULL && (*c)->val != v) c = &(*c)->suiv;
    if (*c == NULL) return 0;                         /* absente */
    Noeud *a_liberer = *c;
    *c = (*c)->suiv;
    free(a_liberer);
    return 1;
}

void inverser_liste(Noeud **tete)
{
    Noeud *prec = NULL, *c = *tete;
    while (c != NULL) {
        Noeud *s = c->suiv;
        c->suiv = prec;
        prec = c;
        c = s;
    }
    *tete = prec;
}

void afficher_liste(Noeud *tete)
{
    for (; tete != NULL; tete = tete->suiv) printf("%d\r\n", tete->val);
}

void liberer_liste(Noeud *tete)
{
    while (tete != NULL) {
        Noeud *s = tete->suiv;
        free(tete);
        tete = s;
    }
}

/* ====================================================================== */
/* ARBRE BINAIRE DE RECHERCHE (ABR)                                        */
/* gauche < noeud < droite. Parcours infixe = valeurs dans l'ordre trié.   */
/* ====================================================================== */
typedef struct Arbre {
    int cle;
    struct Arbre *g, *d;
} Arbre;

Arbre *abr_inserer(Arbre *a, int cle)       /* usage : racine = abr_inserer(racine, x) */
{
    if (a == NULL) {
        a = malloc(sizeof(Arbre));
        a->cle = cle;
        a->g = a->d = NULL;
        return a;
    }
    if (cle < a->cle)      a->g = abr_inserer(a->g, cle);
    else if (cle > a->cle) a->d = abr_inserer(a->d, cle);
    /* égal : déjà présent, on ne fait rien (ou compter les doublons) */
    return a;
}

int abr_contient(Arbre *a, int cle)
{
    while (a != NULL) {
        if (cle == a->cle) return 1;
        a = (cle < a->cle) ? a->g : a->d;
    }
    return 0;
}

void abr_infixe(Arbre *a)                    /* affiche trié */
{
    if (a == NULL) return;
    abr_infixe(a->g);
    printf("%d\r\n", a->cle);
    abr_infixe(a->d);
}

int abr_hauteur(Arbre *a)                    /* arbre vide : 0 */
{
    if (a == NULL) return 0;
    int hg = abr_hauteur(a->g), hd = abr_hauteur(a->d);
    return 1 + (hg > hd ? hg : hd);
}

int abr_taille(Arbre *a)
{
    if (a == NULL) return 0;
    return 1 + abr_taille(a->g) + abr_taille(a->d);
}

void abr_liberer(Arbre *a)
{
    if (a == NULL) return;
    abr_liberer(a->g);
    abr_liberer(a->d);
    free(a);
}

int main(void)
{
    printf("%d %d\r\n", parentheses_ok("([]{})"), parentheses_ok("(]"));

    Noeud *l = NULL;
    inserer_trie(&l, 5); inserer_trie(&l, 1); inserer_trie(&l, 3);
    inverser_liste(&l);
    afficher_liste(l);
    liberer_liste(l);

    Arbre *r = NULL;
    r = abr_inserer(r, 8); r = abr_inserer(r, 3); r = abr_inserer(r, 10);
    abr_infixe(r);
    abr_liberer(r);
    return 0;
}
