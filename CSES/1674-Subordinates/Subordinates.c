#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);
	int compagny[n];
	for (int i=0; i<n; i++) {
		compagny[i] = 0;
	}
	int boss;
	for (int i=0; i<n-1; i++) {
		scanf("%d", &boss);
		for (int j=0;j<boss;j++) {
			compagny[j] += 1;
		}
	}
	for (int i=0; i<n;i++) {
		printf("%d ", compagny[i]);
	}
	printf("\n");
	return 0;
}
