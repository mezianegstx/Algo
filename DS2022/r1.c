#include <stdio.h>


void printarray(int array[], int begin, int end) {
	for (int i=begin;i<=end;i++) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

void swap(int *a, int *b) {int tmp = *a; *a = *b; *b = tmp; }

void quicksort(int array[], int begin, int end) {
	//printarray(array, begin, end);
	if (end-begin < 1) {
		//printf("OK : small\n");
		return;
	}

	int pivot = array[end];
	int i = begin;
	int j = end - 1;
	while (i < j) {
		while (array[i] < pivot) {i++;}
		//printf("i=%d\n", i);
		while (j > begin && array[j] > pivot) {j--;}
		//printf("j=%d\n", j);
		if (i < j) {
			swap(array + i, array + j);
			i++;
			j--;
		}
		//printarray(array, begin, end);
	}
	if (array[i] < pivot) {i++;}
	swap(array + i, array + end); //pivot à sa position finale
	//printarray(array, begin, end);
	quicksort(array, begin, i-1);
	quicksort(array, i+1, end);

}

int main(void) {
	//int array[10] = {3, 5, 8, 1 , 2, 9, 4, 7, 6, 5};
	int n;
	scanf("%d", &n);
	
	int array[n];
	for (int i=0;i<n;i++) {
		scanf("%d", array + i);
	}

	if (n==1) {
		printf("%d\r\n", array[0]);
		return 0;
	}
	quicksort(array, 0, n-1);
	//printarray(array, 0, n-1);
	
	int nb_max = 0;
	int nb = 1;
	int res = -2147483648;
	for (int i=1; i<n; i++) {
		if (array[i-1]==array[i]) {
			nb++;
		} else {nb=1;}
		if (nb >= nb_max && array[i]>=res) {
			res = array[i];
			nb_max = nb;
		}
	}

	printf("%d\r\n", res);

	return 0;
}
