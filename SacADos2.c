#include <stdio.h>
#include <stdlib.h>

void printArray(int bag, int count, int array[bag*count], int ok, int no) {
	printf("\n\t");
	for (int j = 0; j < count; j++) {
		printf("obj%d\t", j+1);
	}
	printf("\n");

	for (int i = 0; i < bag; i++) {
		printf("c%d\t", i);
		for (int j = 0; j < count; j++) {
			int val = array[i*count+j];
			if (val == ok) {
				printf("OK\t");
			} else if (val == no) {
				printf("NON\t");
			} else {
				int i2 = val / count;
				int j2 = val % count;
				printf("(%d,%d)\t", i2, j2);
			}
		}
		printf("\n");
	}
}

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
	
	int array[(bag+1)*count];

	// ----------------------------------------
	

	int ok = -count-1;
	int no = -count-2;
	//printf("\nCapacité : %d\n", bag);
	for (int i = 0; i<count;i++) {
		array[i] = ok;
		for (int j=1;j<=bag;j++) {
			array[j*count + i] = no;
		}
		//printf("Objet %d : %d\n", i+1, objects[i]);
	}

	//printf("\n");
	
	// ---------------------------------------
	
	int NotUsedAlready(int prec, int objectid) {
		while (1) {
			if (prec==ok) {
				return 1;
			}
			if (prec % count == objectid) {
				return 0;
			}
			//if (prec == ok) {
			//	return 1;
			//}
			//if (prec == no) {
				//printf("Erreur prec == no\n");
			//	return 0;
			//}
			prec = array[prec];
		}
	}

	for (int c=1;c<=bag;c++) {
		for (int o=0;o<count;o++) {
			if (c - objects[o] >= 0) {
				for (int i=0;i<count;i++) {
					if (array[(c-objects[o])*count + i] != no) {
						if (c==objects[o]) {
                                                                array[c*count + o] = o;
                                                                break;
                                                        }
						if (i!=o && NotUsedAlready(array[(c-objects[o])*count + i], o)) {
							//if (c==objects[o]) {
							//	array[c*count + o] = o;
							//	break;
							//}
							array[c*count + o] = (c-objects[o])*count + i;
							break;
						}
					}
				}
			}
		}
	}

	printArray(bag+1, count, array, ok, no);

	for (int i=0; i<count;i++) {
		if (array[bag*count + i] != no) {
			printf("OUI\r\n");
			return 0;
		}
	}
	printf("NON\r\n");


	return 0;
}
