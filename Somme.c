#include <stdio.h>

int somme(void) {
        int a, b, c;
	printf("Entrez 3 nombres : ");
	scanf("%d %d %d", &a, &b, &c);
	// printf("%d, %d, %d\n", a, b, c);
	return a + b + c;
}

int main(void) {
	printf("Résultat: %d\n", somme());
	return 0;
}
