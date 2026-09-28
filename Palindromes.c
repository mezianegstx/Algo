#include <stdio.h>
#include <stdlib.h>

int main(void) {
	int v;
	int size = 4;
	int count = 0;
	int *vec = malloc(size);
	scanf("%d", &v);
	while (v!=-1){
		if (count == size) {
			size *= 2;
			//printf("size : %d", size);
			vec = realloc(vec, size*sizeof(int));
		}
		vec[count++] = v;
		scanf("%d", &v);
	}
	
	for (int i=0; i<count/2; i++) {
		// printf("%d <-> %d\n", vec[i], vec[size-i-1]);
		if (vec[i] != vec[count-i-1]) {
			printf("0\r\n");
			return 0;
		}
	}
	
	printf("1\r\n");
	return 0;
}

