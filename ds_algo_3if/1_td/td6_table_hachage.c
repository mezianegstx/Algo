/*
 * TD6 : Table de hachage à adressage ouvert (sondage linéaire)
 * ============================================================
 * ENONCE : commandes
 *   init <n>              alloue une table de n cases
 *   insert <cle> <valeur> ajoute, ou ECRASE la valeur si la clé existe
 *   delete <cle>          passe la case à l'état REMOVED
 *   query <cle>           affiche la valeur, ou "Not found"
 *   stats                 affiche size / empty / deleted / used
 *   destroy               libère la mémoire
 *   bye                   quitte
 *
 * NOTE : l'ossature moodle fournit la lecture des commandes et la fonction de
 *   hachage. Ici tout est réécrit pour avoir un programme complet. Si tu pars
 *   de l'ossature, garde SA HashFunction et recopie Insert/Delete/Query/Stats.
 *   (Les résultats de stats ne dépendent pas de la fonction de hachage choisie.)
 *
 * PRINCIPE :
 *   On calcule h = HashFunction(cle, size). Si la case h est prise par une
 *   autre clé, on essaie h+1, h+2, ... (modulo size) : c'est le sondage linéaire.
 *   Chaque case a un état : EMPTY (jamais utilisée), SET (occupée),
 *   REMOVED (occupée puis supprimée).
 *
 * POURQUOI L'ETAT REMOVED ? (point clé du TD)
 *   Si on remettait une case supprimée à EMPTY, une recherche s'arrêterait
 *   dessus et ne trouverait plus les clés placées APRES elle lors d'une
 *   collision. Donc :
 *   - RECHERCHE (query/delete) : on saute les REMOVED, on s'arrête sur EMPTY.
 *   - INSERTION : on retient la 1re case REMOVED rencontrée, MAIS on continue
 *     à chercher jusqu'à EMPTY pour vérifier que la clé n'est pas plus loin
 *     (sinon on aurait la même clé deux fois). Si on la trouve -> on écrase.
 *     Sinon on insère dans la 1re REMOVED (ou à défaut dans la case EMPTY).
 *
 * FORMAT DE STATS (8 caractères avant le ':') :
 *   "size    : 100"   (4 espaces)
 *   "empty   : 98"    (3 espaces)
 *   "deleted : 1"     (1 espace)
 *   "used    : 1"     (4 espaces)
 *
 * CAS PARTICULIERS / PIEGES :
 *   - insert d'une clé existante : écraser la valeur (free l'ancienne),
 *     ne PAS créer un 2e élément. "used" ne bouge pas.
 *   - Clé supprimée puis réinsérée : elle peut prendre la case REMOVED.
 *   - delete d'une clé absente : ne rien faire.
 *   - Table pleine (que des SET/REMOVED) : la boucle fait au plus size tours,
 *     sinon boucle infinie ! Si aucune place : on ignore l'insertion.
 *   - strcmp pour comparer des chaînes (JAMAIS ==, qui compare des adresses).
 *   - Copier les chaînes (strdup / malloc + strcpy) : le buffer de lecture est
 *     réutilisé à chaque commande, stocker son adresse serait une erreur.
 *   - init alors qu'une table existe déjà : on libère l'ancienne d'abord.
 *   - Commandes avant init ou après destroy : ne pas planter (tests != NULL).
 *   - Chaque ligne affichée finit par "\r\n".
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EMPTY   0
#define SET     1
#define REMOVED 2

const char *Labels[] = {"Empty", "Set", "Removed"};   /* utile pour débugger */

typedef char *Key;

typedef struct {
    char *key;
    int   status;
    char *val;
} Data;

typedef struct {
    Data *tab;
    int   size;
} HashTable;

/* Copie d'une chaîne (strdup n'existe pas sur tous les compilateurs) */
char *copier_chaine(const char *s)
{
    char *copie = malloc(strlen(s) + 1);
    strcpy(copie, s);
    return copie;
}

/* Fonction de hachage (djb2). Celle de l'ossature fait la même chose. */
unsigned int HashFunction(Key key, unsigned int size)
{
    unsigned int h = 5381;
    int i;
    for (i = 0; key[i] != '\0'; i++) {
        h = h * 33 + (unsigned char)key[i];
    }
    return h % size;
}

void Init(HashTable *t, int size)
{
    int i;
    t->size = size;
    t->tab = malloc(size * sizeof(Data));
    for (i = 0; i < size; i++) {
        t->tab[i].status = EMPTY;
        t->tab[i].key = NULL;
        t->tab[i].val = NULL;
    }
}

void Insert(HashTable *t, char *key, char *val)
{
    if (t->tab == NULL || t->size <= 0) return;

    unsigned int h = HashFunction(key, t->size);
    int premier_removed = -1;   /* 1re case supprimée rencontrée */
    int case_vide = -1;         /* case EMPTY qui arrête la recherche */
    int i;

    for (i = 0; i < t->size; i++) {              /* au plus size essais */
        int idx = (h + i) % t->size;
        Data *d = &t->tab[idx];

        if (d->status == EMPTY) {
            case_vide = idx;                     /* la clé n'est pas plus loin */
            break;
        }
        if (d->status == REMOVED) {
            if (premier_removed == -1) premier_removed = idx;
            continue;                            /* on continue de chercher ! */
        }
        /* status == SET */
        if (strcmp(d->key, key) == 0) {          /* clé déjà là : on écrase */
            free(d->val);
            d->val = copier_chaine(val);
            return;
        }
    }

    /* la clé n'existe pas : on choisit la place */
    int place = (premier_removed != -1) ? premier_removed : case_vide;
    if (place == -1) return;                     /* table pleine */

    Data *d = &t->tab[place];
    free(d->key);                                /* au cas où (REMOVED déjà libéré) */
    free(d->val);
    d->key = copier_chaine(key);
    d->val = copier_chaine(val);
    d->status = SET;
}

/* Renvoie l'indice de la case qui contient key, ou -1 */
int Find(HashTable *t, char *key)
{
    if (t->tab == NULL || t->size <= 0) return -1;

    unsigned int h = HashFunction(key, t->size);
    int i;
    for (i = 0; i < t->size; i++) {
        int idx = (h + i) % t->size;
        Data *d = &t->tab[idx];
        if (d->status == EMPTY) return -1;       /* fin de la chaîne de sondage */
        if (d->status == SET && strcmp(d->key, key) == 0) return idx;
        /* REMOVED ou autre clé : on continue */
    }
    return -1;
}

void Delete(HashTable *t, char *key)
{
    int idx = Find(t, key);
    if (idx == -1) return;                       /* absente : rien */
    Data *d = &t->tab[idx];
    free(d->key);
    free(d->val);
    d->key = NULL;
    d->val = NULL;
    d->status = REMOVED;                         /* PAS EMPTY ! */
}

void Query(HashTable *t, char *key)
{
    int idx = Find(t, key);
    if (idx == -1) printf("Not found\r\n");
    else           printf("%s\r\n", t->tab[idx].val);
}

void Stats(HashTable *t)
{
    int vides = 0, supprimees = 0, utilisees = 0, i;
    for (i = 0; i < t->size; i++) {
        if (t->tab[i].status == EMPTY)        vides++;
        else if (t->tab[i].status == REMOVED) supprimees++;
        else                                  utilisees++;
    }
    printf("size    : %d\r\n", t->size);
    printf("empty   : %d\r\n", vides);
    printf("deleted : %d\r\n", supprimees);
    printf("used    : %d\r\n", utilisees);
}

void Destroy(HashTable *t)
{
    int i;
    if (t->tab == NULL) return;
    for (i = 0; i < t->size; i++) {
        free(t->tab[i].key);
        free(t->tab[i].val);
    }
    free(t->tab);
    t->tab = NULL;
    t->size = 0;
}

int main(void)
{
    HashTable table;
    char commande[64];
    static char cle[100000];      /* static : gros buffers hors de la pile */
    static char valeur[100000];
    int n;

    table.tab = NULL;
    table.size = 0;

    while (scanf("%63s", commande) == 1) {
        if (strcmp(commande, "init") == 0) {
            scanf("%d", &n);
            Destroy(&table);                     /* si déjà initialisée */
            if (n > 0) Init(&table, n);
        } else if (strcmp(commande, "insert") == 0) {
            scanf("%99999s %99999s", cle, valeur);
            Insert(&table, cle, valeur);
        } else if (strcmp(commande, "delete") == 0) {
            scanf("%99999s", cle);
            Delete(&table, cle);
        } else if (strcmp(commande, "query") == 0) {
            scanf("%99999s", cle);
            Query(&table, cle);
        } else if (strcmp(commande, "stats") == 0) {
            Stats(&table);
        } else if (strcmp(commande, "destroy") == 0) {
            Destroy(&table);
        } else if (strcmp(commande, "bye") == 0) {
            break;
        }
    }

    Destroy(&table);
    return 0;
}
