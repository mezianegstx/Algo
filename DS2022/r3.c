#include <stdio.h>

int main(void) {
	int n;
	scanf("%d", &n);

	int array[n];
	int i;
	for (i=0; i<n; i++) {
		array[i]=0;
	}

	for (int d=2; d<n/2+1;d++) {
		i = 2;
		while (i*d<=n) {
			array[i*d-1] += d;
			i++;
		}
	}
	
	int count=0;
	for (i=0;i<n;i++) {
		if (array[i] >= i+1) {
			count++;
		}
	}

	printf("%d\r\n", count);

	return 0;
}
