#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int val;
	int pos;
} Valeur;

void swap(Valeur *a, Valeur *b) {
	Valeur tmp = *a;
	*a = *b;
	*b = tmp;
}

int cmp(const void *a, const void *b) {
	int va = ((const Valeur *)a)->val;
	int vb = ((const Valeur *)b)->val;
	return (va > vb) - (va < vb);
}

void quicksort(Valeur tab[], int a, int b) {
	
	if (b<=a) {
		return;
	}

	int pivot = tab[b].val;
	int l=a;
	int r=b-1;
	while(l<r) {
		while (tab[l].val<pivot) {l++;}
		while (r>a && tab[r].val>=pivot) {r--;}
		if (l<r) {
			swap(&tab[l], &tab[r]);
		}
	}
	if (tab[l].val>pivot) {
		swap(&tab[l], &tab[b]);
	}

	quicksort(tab, a, l-1);
	quicksort(tab, l+1, b);
}

void printtab(const Valeur tab[], int n) {
	for (int i=0; i<n; i++) {
		printf("%d   %d\n", tab[i].pos, tab[i].val);
	}
	printf("\n");
}

int main(void) {
	int n, x;
	scanf("%d %d", &n, &x);
	Valeur tab[n];
	for (int i=0; i<n; i++) {
		tab[i].pos = i;
		scanf("%d", &tab[i].val);

	}
	//quicksort(tab, 0, n-1);
	qsort(tab, n, sizeof(Valeur), cmp);
	int l = 0;
	int r = n-1;
	int s;
	while (l<r) {
		s = tab[l].val+tab[r].val;
		if (s == x) {
			printf("%d %d\n", tab[l].pos+1, tab[r].pos+1);
			return 0;
		}
		if (s>x) {
			r--;
		} else {l++;}
	}
	printf("IMPOSSIBLE\n");

	//swap(&a, &b);
	
	return 0;
}
