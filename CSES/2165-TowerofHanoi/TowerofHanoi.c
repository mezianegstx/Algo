#include <stdio.h>

void hanoi(int disk, int from, int to) {
	if (disk==1) {
		printf("%d %d\r\n", from, to);
		return;
	}
	hanoi(disk-1, from, 6-from-to);
	printf("%d %d\r\n", from, to);
	hanoi(disk-1, 6-from-to, to);
}

int main(void) {
	int n;
	scanf("%d", &n);
	int s = 2;
	for (int i=1; i<n; i++) 
	{
		s *= 2;
	}
	s--;
	printf("%d\r\n", s);
	hanoi(n, 1, 3);

	return 0;
}
