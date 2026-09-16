#include <stdio.h>
#include <stdlib.h>



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

        char reachable[capacity+1];
	reachable[0] = 1;
	for (int i = 1; i <= capacity; i++) {
    		reachable[i] = 0;
	}
	
	for (int c = 0; c <= capacity; c++) {
		for (int i = 0; i <= size; i++) {
			if (objects[i] <= c && reachable[c - objects[i]]) {
				reachable[c] = 1;
				break;
			}
		}
	}
	
	if (reachable[capacity]) {
		printf("OUI\n");
	} else {
		printf("NON\n");
	}

	free(objects);
        return 0;
}
