#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);
	int a = 1;
	int i = 1;
	while (n > a) {
		a = a + 4*(++i-1) + 1;
	}
	if (a==n) {
		printf("O\n");
	} else {
		printf("N\n");
	}
	return 0;
}
