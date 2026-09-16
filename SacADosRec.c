#include <stdio.h>
#include <stdlib.h>

int sad(int capacity, int* objects, int n) {
	if (capacity == 0) {
		return 1;
	}
	for (int i=0; i<n; i++){
		if (capacity>=objects[i]) {
			if (sad(capacity-objects[i], objects, n)){
				return 1;
			}
		}
	}
	return 0;
}

int main(void) {
	int capacity;
	scanf("%d", &capacity);

        int v;
        int size = 0;
        int *objects = malloc(size);
        scanf("%d", &v);
        while (v!=-1){
                objects = realloc(objects, (size + 1) * sizeof(int));
                objects[size++] = v;
                scanf("%d", &v);
        }

	if (sad(capacity, objects, size)) {
		printf("OUI\n");
	} else {
		printf("NON\n");
	}

	return 0;
}
