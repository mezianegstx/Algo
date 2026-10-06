/*
 * TD5 : File (FIFO) dans un tableau circulaire de taille 100
 * ==========================================================
 * ENONCE : commandes "queue <n>", "dequeue", "bye".
 *   dequeue affiche et retire le plus ancien élément. File vide -> rien.
 *   Exemple : queue 1, queue 2, queue 3, dequeue x3 -> 1 2 3
 *
 * IDEE : un tableau de 100 cases + deux infos :
 *   debut = indice du plus ancien élément
 *   nb    = nombre d'éléments présents
 *   - enfiler  : on écrit à l'indice (debut + nb) % MAX, puis nb++
 *   - défiler  : on lit tab[debut], puis debut = (debut + 1) % MAX, nb--
 *   Le "% MAX" fait revenir à 0 après la case 99 : c'est ça le "circulaire".
 *
 * POURQUOI garder nb plutôt que (debut, fin) seulement ?
 *   Avec juste debut et fin, "debut == fin" peut vouloir dire vide OU plein.
 *   Avec nb, aucune ambiguïté : vide si nb == 0, pleine si nb == MAX.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - dequeue sur file vide : n'afficher RIEN.
 *   - queue et dequeue mélangés : le circulaire gère (plus de 100 insertions
 *     au total possibles tant qu'il n'y en a jamais plus de 100 en même temps).
 *   - File pleine : non spécifié, on ignore l'insertion.
 *   - Valeurs négatives : "%d" les gère.
 *   - Ne PAS décaler tout le tableau à chaque dequeue (O(n)) : c'est
 *     justement ce que le circulaire évite (O(1)).
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct {
    int tab[MAX];
    int debut;   /* indice du plus ancien */
    int nb;      /* nombre d'éléments     */
} File;

void init_file(File *f)
{
    f->debut = 0;
    f->nb = 0;
}

int est_vide(File *f)  { return f->nb == 0; }
int est_pleine(File *f) { return f->nb == MAX; }

void enfiler(File *f, int valeur)
{
    if (est_pleine(f)) return;                  /* non spécifié : on ignore */
    f->tab[(f->debut + f->nb) % MAX] = valeur;
    f->nb++;
}

/* renvoie 1 et met la valeur dans *res, ou 0 si vide */
int defiler(File *f, int *res)
{
    if (est_vide(f)) return 0;
    *res = f->tab[f->debut];
    f->debut = (f->debut + 1) % MAX;
    f->nb--;
    return 1;
}

int main(void)
{
    File f;
    char commande[64];
    int valeur;

    init_file(&f);

    while (scanf("%63s", commande) == 1) {
        if (strcmp(commande, "queue") == 0) {
            scanf("%d", &valeur);
            enfiler(&f, valeur);
        } else if (strcmp(commande, "dequeue") == 0) {
            if (defiler(&f, &valeur)) {
                printf("%d\r\n", valeur);
            }
        } else if (strcmp(commande, "bye") == 0) {
            break;
        }
    }
    return 0;
}
