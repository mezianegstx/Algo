#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);
	char chaine[101];
	int count = 0;
	int vus = 0;
	int bit = 0;
	for (int i=0;i<n;i++) {
		scanf("%100s", chaine);
		vus=0;
		int j=0;
		while (chaine[j] != '\0') {
			bit = 1 << chaine[j++] - 'A';
			if (vus & bit) {
				count++;
				break;
			}
		vus |= bit;
		}
	}

	printf("%d\n", count);

	return 0;
}
