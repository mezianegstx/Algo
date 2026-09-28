#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int allocated; /* current allcoation of array */
  int filled;    /* number of items present in the binheap */
  int *array;    /* array of values */
} BinaryHeap;

/* Init allocates the structure BinaryHeap and
 * also the membre array with the given size 
 * it also fill allocated (size) and intializes 
 * filled to 0 */
BinaryHeap * Init(int size);

/* InsertValue insert value into the binary heap
 * the array is reallocated if necessary (allocated changed 
 * with respect to the new size )
 * filled is incremented by 1 */
void InsertValue(BinaryHeap * heap, int value);

/* ExtractMAx returns 0 if the binary heap is empty
 * otherwise it return 1 and fills *val with the maximum 
 * value present in the binary heap
 * filled is decremented by 1  and the max value is removed
 * from the binary heap */
int ExtractMax(BinaryHeap * heap, int * res);


int GetBiggerChild(BinaryHeap * heap, int daddy);

//int IsHeapEmpty(BinaryHeap * heap);

void PrintArray(BinaryHeap * heap);

/* Destroy frees the structure and the array */
void Destroy(BinaryHeap * heap);


int main(void) 
{
  char lecture[100];
  int val;
  BinaryHeap * heap;
  heap = Init(10);

  fscanf(stdin,"%99s",lecture);
  while (strcmp(lecture,"bye")!=0) {
    if (strcmp(lecture,"insert")==0) {
      fscanf(stdin,"%99s",lecture);
      val = strtol(lecture,NULL,10);
      InsertValue(heap,val);
    } else if (strcmp(lecture,"extract")==0) {
      if(ExtractMax(heap,&val))
      {
        printf("%d\r\n",val);
      }
    } else if (strcmp(lecture,"print")==0) {
    	PrintArray(heap);
    }
    fscanf(stdin,"%99s",lecture);
  }
  Destroy(heap);
  return 0;
}

BinaryHeap * Init(int size)
{
  BinaryHeap * heap;
  heap = malloc(sizeof(BinaryHeap));
  heap->allocated = size;
  heap->filled = 0;
  heap->array = malloc(sizeof(int)*size);
  return heap;
}

void InsertValue(BinaryHeap * heap, int value)
{
	if (heap->filled==heap->allocated) {
                heap->allocated *= 2;
		heap->array = (int*) realloc(heap->array, sizeof(int)*heap->allocated);
        }
        heap->array[heap->filled] = value;
        int i = heap->filled;
        heap->filled += 1;
        int daddy = (i-1)/2;
        while (i>0 && heap->array[daddy] < heap->array[i]) {
                int tmp = heap->array[i];
                heap->array[i] = heap->array[daddy];
                heap->array[daddy] = tmp;
                i = daddy;
                daddy = (i-1)/2;
        }
}

int ExtractMax(BinaryHeap * heap, int *res ) {
	if (heap->filled==0) return 0;
	//PrintArray(heap);
	*res = heap->array[0];
	heap->filled -=1;
        heap->array[0] = heap->array[heap->filled];

	//PrintArray(heap);
        int daddy = 0;
        int bchild = GetBiggerChild(heap, daddy);
        while (bchild!=-1 && heap->array[daddy]<heap->array[bchild]) {
                int tmp = heap->array[bchild];
                heap->array[bchild] = heap->array[daddy];
                heap->array[daddy] = tmp;
                daddy = bchild;
                bchild = GetBiggerChild(heap, daddy);
		//PrintArray(heap);
        }
        return 1;
}

int GetBiggerChild(BinaryHeap * heap, int daddy) {
	if (2*daddy+1>heap->filled) {
		return -1;
	}
	if (2*daddy+2>heap->filled) {
		return 2*daddy +1;
	}
        int child1 = heap->array[2*daddy +1];
        int child2 = heap->array[2*daddy +2];
        if (child1>child2) {
                return 2*daddy +1;
        }
        return 2*daddy +2;
}

//int IsHeapEmpty(BinaryHeap * heap) {
  //      if (heap->filled==0) return 1;
    //    return 0;
//}

void PrintArray(BinaryHeap * heap) {
        if (heap->allocated==0) {
                printf("La pile est vide\n");
        } else {

        //Maillon *m = p->sommet;

        int largeur;
        int i;
        printf("┌─");
        for (i=0;i<heap->allocated;i++){
                largeur = snprintf(NULL, 0, "%d", heap->array[i]);
                printf("%.*s", largeur*3, "─────────────────────────────────");
                if (i==heap->allocated-1) {
                        printf("─┐\n");
                } else {
                        printf("─┬─");
                }

        }
        //m = p->sommet;
        printf("│ ");
        for (i=0;i<heap->allocated;i++){
                printf("%d", heap->array[i]);
                if (i==heap->allocated-1) {
                        printf(" │\n");
                } else {
                        printf(" │ ");
                }

        }

        //m = p->sommet;
        printf("└─");
        for (i=0;i<heap->allocated;i++){
                largeur = snprintf(NULL, 0, "%d", heap->array[i]);
		if (i==heap->filled-1) {
			printf("^");
			printf("%.*s", (largeur-1)*3, "─────────────────────────────────");
		} else {
                printf("%.*s", largeur*3, "─────────────────────────────────");
		}
		if (i==heap->allocated-1) {
                        printf("─┘\n");
                } else {
                        printf("─┴─");
                }

        }
	}
}

void Destroy(BinaryHeap * heap)
{
	if (heap) {
		free(heap->array);
		free(heap);
	}
}


