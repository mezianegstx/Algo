/*
 * TD4 : Tas binaire MAX (tableau dynamique)
 * =========================================
 * ENONCE : commandes "insert <n>", "extract", "bye".
 *   extract affiche le max et le retire. Tas vide -> extract n'affiche RIEN.
 *   Exemple : insert 15, 4, 8, 9, 2, 18 puis 7 extract -> 18 15 9 8 4 2
 *
 * NOTE : l'ossature moodle fournit la lecture des commandes. Ici le programme
 *   est COMPLET (lecture comprise) pour pouvoir le soumettre tel quel.
 *   Si tu pars de l'ossature, recopie juste les fonctions et adapte les noms.
 *
 * RAPPELS (indices dans le tableau, racine en 0) :
 *   fils gauche de i = 2*i + 1
 *   fils droit  de i = 2*i + 2
 *   parent      de i = (i - 1) / 2
 *   Propriété de tas MAX : chaque parent >= ses fils -> le max est en array[0].
 *
 * INSERTION ("percolation vers le haut") :
 *   1. agrandir le tableau s'il est plein (realloc, taille x2)
 *   2. mettre la valeur à la fin (case filled)
 *   3. tant que pas à la racine et parent < valeur : échanger avec le parent
 *
 * EXTRACTION DU MAX ("percolation vers le bas") :
 *   1. le max est array[0] (on le garde pour l'afficher)
 *   2. on met la DERNIERE valeur à la racine, filled--
 *   3. tant que la case a au moins un fils :
 *        prendre le PLUS GRAND des deux fils (attention : le fils droit
 *        peut ne pas exister !). S'il est > à la case, échanger et descendre,
 *        sinon stop.
 *
 * CAS PARTICULIERS / PIEGES :
 *   - extract sur tas vide : n'afficher rien, ne pas planter.
 *   - Toujours vérifier que le fils existe (indice < filled) AVANT de le lire.
 *   - Comparer avec le PLUS GRAND des deux fils, sinon le tas est cassé.
 *   - Valeurs en double (insert 5 deux fois) : utiliser > et pas >= dans
 *     les tests d'échange, ça marche dans tous les cas.
 *   - Allocation initiale > 0 (sinon le doublement 0 * 2 = 0 boucle).
 *   - Pour un tas MIN : inverser toutes les comparaisons.
 *   - Penser à free à la fin.
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int allocated;   /* nombre de cases allouées    */
    int filled;      /* nombre de cases remplies    */
    int *array;      /* les valeurs, racine en [0]  */
} BinaryHeap;

BinaryHeap *Init(int size)
{
    BinaryHeap *heap = malloc(sizeof(BinaryHeap));
    if (size < 1) size = 1;                 /* jamais 0 (doublement) */
    heap->allocated = size;
    heap->filled = 0;
    heap->array = malloc(size * sizeof(int));
    return heap;
}

void echanger(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void InsertValue(BinaryHeap *heap, int value)
{
    /* 1. agrandir si plein */
    if (heap->filled == heap->allocated) {
        heap->allocated = heap->allocated * 2;
        heap->array = realloc(heap->array, heap->allocated * sizeof(int));
    }

    /* 2. placer à la fin */
    int i = heap->filled;
    heap->array[i] = value;
    heap->filled++;

    /* 3. remonter tant que le parent est plus petit */
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap->array[parent] < heap->array[i]) {
            echanger(&heap->array[parent], &heap->array[i]);
            i = parent;
        } else {
            break;
        }
    }
}

/* Retire le max et le met dans *res. Renvoie 0 si le tas est vide, 1 sinon. */
int ExtractMax(BinaryHeap *heap, int *res)
{
    if (heap->filled == 0) return 0;          /* tas vide */

    *res = heap->array[0];                    /* le max */
    heap->filled--;
    heap->array[0] = heap->array[heap->filled]; /* dernier -> racine */

    /* descendre */
    int i = 0;
    while (1) {
        int gauche = 2 * i + 1;
        int droite = 2 * i + 2;
        int plus_grand = i;

        if (gauche < heap->filled && heap->array[gauche] > heap->array[plus_grand])
            plus_grand = gauche;
        if (droite < heap->filled && heap->array[droite] > heap->array[plus_grand])
            plus_grand = droite;

        if (plus_grand == i) break;           /* propriété respectée */
        echanger(&heap->array[i], &heap->array[plus_grand]);
        i = plus_grand;
    }
    return 1;
}

void Destroy(BinaryHeap *heap)
{
    free(heap->array);
    free(heap);
}

int main(void)
{
    char commande[64];
    int valeur;
    BinaryHeap *heap = Init(10);

    while (scanf("%63s", commande) == 1) {
        if (strcmp(commande, "insert") == 0) {
            scanf("%d", &valeur);
            InsertValue(heap, valeur);
        } else if (strcmp(commande, "extract") == 0) {
            if (ExtractMax(heap, &valeur)) {
                printf("%d\r\n", valeur);
            }                                 /* vide : rien */
        } else if (strcmp(commande, "bye") == 0) {
            break;
        }
    }

    Destroy(heap);
    return 0;
}
