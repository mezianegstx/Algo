#include <stdio.h>

int main(void) {
	int a1, b1, a2, b2;
	scanf("%d", &a1);
	scanf("%d", &b1);
	scanf("%d", &a2);
	scanf("%d", &b2);

	int aire;
	if (a1 < a2) {
		if (b1 < b2) {
			aire = b1 - a2;
		} else {
			aire = b2 - a2;
		}
	} else {
		if (b1 < b2) {
			aire = b1 - a1;
		} else {
			aire = b2 - a1;
		}
	}
	
	if (aire < 0) {
		aire = 0;
	}

	printf("Aire : %d\n", aire);

	return 0;
}
