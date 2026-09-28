#include <stdio.h>
#include <stdlib.h>

int main () {
	int bag;
	scanf("%d", &bag);
	
	int size = 4;
	int * objects;
	objects = malloc(size*sizeof(int));
	int count = 0;
	int v;
	scanf("%d", &v);
	while (v!=-1) {
		if (count==size) {
			size *=2;
			objects = realloc(objects, size*sizeof(int));
		}
		objects[count++] = v;
		scanf("%d", &v);
	}
	
	int array[bag+1];
	array[0] = 1;
	for (int i=1;i<=bag;i++) {array[i]=0;}

	for (int o=0;o<count;o++) {
		for (int c=bag;c>=objects[o];c--) {
			if (c-objects[o] >= 0 && array[c-objects[o]] == 1) {
		array[c] = 1;		
			}
		}
	}
	
	if (array[bag] == 1) {
		printf("OUI\r\n");
		return 0;
	}
	
	printf("NON\r\n");


	return 0;
}
