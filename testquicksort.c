#include "QuickSort.h"
#include <stdio.h>

int main(void) {
	int tab[5] = {3, 1, 4, 1, 5};
	quicksort(tab, 0, 4);
	for (int i = 0; i<5; i++) {
		printf("%d ", tab[i]);
	}

	printf("\r\n");

	return 0;
}
