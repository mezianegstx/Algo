#include <stdio.h>

void echange(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

/* Trie tab[debut..fin] (bornes incluses) par ordre croissant */
void quicksort(int *tab, int debut, int fin) {
    if (debut >= fin) return;
    int pivot = tab[(debut + fin) / 2];
    int i = debut, j = fin;
    while (i <= j) {
        while (tab[i] < pivot) i++;
        while (tab[j] > pivot) j--;
        if (i <= j) {
            echange(&tab[i], &tab[j]);
            i++;
            j--;
        }
    }
    quicksort(tab, debut, j);
    quicksort(tab, i, fin);
}

double m1(int valeurs[], int n) {
	double m = 0;
	for (int i=0;i<n/10;i++) {
		//printf("\n%d %d\n", valeurs[i], valeurs[n-i-1]);
		m += (double)valeurs[i] / (n/5);
		m += (double)valeurs[n-i-1] / (n/5);
	}

	return m;
}

double m2(int valeurs[], int n) {
	double m = 0;
	for (int i=0; i<n; i++) {
		m += (double)valeurs[i] / n;
	}
	return m;
}

int main(void) {
	int n;
	scanf("%d", &n);

	int valeurs[n];
	for (int i=0; i<n; i++) {
		scanf("%d", &valeurs[i]);
	}

	quicksort(valeurs, 0, n-1);
	
	double m11 = m1(valeurs, n);
	double m22 = m2(valeurs, n);
	
	//printf("\n%f %f\n", m11, m22);

	if ((m11 > m22 && m11 - m22 < 2) || (m22 > m11 && m22 - m11 < 2)) {
		printf("OUI\r\n");
	} else {
		printf("NON\r\n");
	}

	return 0;
}
