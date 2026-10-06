#include <stdio.h>

int findPlace(const int tab[], int a, int b, int v) {
	int mid;
	while (b>a) {
		mid = a + (b-a)/2;
		if (tab[mid] <= v) {
			a = mid+1;
		} else {
			b = mid;
		}
	}
        return a;
}

int main(void) {
	int n;
	scanf("%d", &n);
	int towers[n];
	int c = 0;
	int p;
	int t;
	for (int i=0; i<n; i++) {
		scanf("%d", &t);
		
		p = findPlace(towers, 0, c, t);
		towers[p] = t;
		if (p==c) {
			c++;
		}
	}

	printf("%d\r\n", c);
	return 0;
}
