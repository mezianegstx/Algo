#include <stdio.h>

int g(int n) {
	int tmp;
	int count = 0;
		while (n / 10 !=  0 && count < 100000) {
			tmp = 0;
			while (n != 0) {
				tmp += n % 10;
				n /= 10;
			}
			n = tmp;
			count++;
		}
	return n;
}

int main(void) {
	int n1, n2;
	scanf("%d\n%d", &n1, &n2);
	if (g(n1)==g(n2)) {
		printf("OUI\r\n");
	} else {
		printf("NON\r\n");
	}
	return 0;
}
