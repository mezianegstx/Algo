#include <stdio.h>
// #include "QuickSort.h"

void swap(int *a, int *b) {int tmp = *a; *a = *b; *b = tmp; }

void quicksort(int tab[], int begin, int end) {
	if (begin >= end) {
		return;
	}
	
	int pivotid = begin + (end - begin) / 2;
	int i = begin;
	int j = end;

	while (i < j) {
		while (tab[i] < tab[pivotid]) {i++;}
		while (tab[pivotid] < tab[j]) {j--;}
		swap((tab + i), (tab + j)); i++; j--;
	}

	quicksort(tab, i, end);
	quicksort(tab, begin, j);
}

int main(void) {
        int tab[5] = {3, 1, 4, 1, 5};
        quicksort(tab, 0, 4);
        for (int i = 0; i<5; i++) {
                printf("%d ", tab[i]);
        }

        printf("\r\n");

        return 0;
}
