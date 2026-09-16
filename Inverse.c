#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);
	for (int i=0;i<n;i++) {
		double x;
		scanf("%lf", &x);
		printf("%f\n", 1.0 / x);
	}
	return 0;
}

