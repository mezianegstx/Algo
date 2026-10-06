#include <stdio.h>

#define MOD 1000000007

int main(void) {
	
	int n;
	scanf("%d", &n);

	int tab[n+1];
	tab[0] = 1;
	for (int i=1; i<=n; i++) {
		tab[i]=0;
	}

	for (int i=1; i<=n;  i++) {
		for (int d=1; d<7; d++) {
			if (d<=i && tab[i-d]) {
				if (MOD - tab[i] < tab[i-d]) {
					tab[i] -= MOD;
				}
				tab[i] += tab[i-d];
				
			}
		}
	}

	printf("%d\n", tab[n]);

	return 0;
}
