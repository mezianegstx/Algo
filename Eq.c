#include <stdio.h>

int main() {
	int a, b;
	scanf("%d", &a);
	scanf("%d", &b);
	
	for (int x=-a; x<=a;x++) {
		for (int y=-b; y<=b;y++) {
			if (x*x + y*y == a && x*x*x + y*y*y == b) {
				printf("%d %d\r\n", x, y);
				printf("%d %d\r\n", y, x);
				return 0;
			}
		}
	}
	printf("-\r\n");
	return 0;
}
