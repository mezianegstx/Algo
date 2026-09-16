#include <stdio.h>

int f(int n) {
	if (n%2 == 0) {
		return n/2;
	} else {
		return 3*n + 1;
	}
}

int get_a(int i, int n) {
	if (i==0) {
		return n;
	} else {
		return f(get_a(i-1, n));
	}
}

int main(void) {
	int n;
	scanf("%d", &n);
	int a = n;
	int i = 0;
	printf("\n");
	while (a != 1) {
		a = get_a(i++, n);
		printf("%d\n", a);
	}
	return 0;
}
