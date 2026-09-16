#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int v;
	int size = 0;
	int *vec = malloc(size);
	scanf("%d", &v);
	while (v!=-1){
		vec = realloc(vec, size + 1);
		vec[size++] = v;
		scanf("%d", &v);
	}
	
	for (int i=0; i<size/2; i++) {
		// printf("%d <-> %d\n", vec[i], vec[size-i-1]);
		if (vec[i] != vec[size-i-1]) {
			printf("Pas un palindrome.\n");
			return 0;
		}
	}
	
	printf("Palindrome !\n");
	return 0;
}

