#include <stdio.h>

int main(void) {
	int c;
	fscanf(stdin, "%d", &c);
	int na, nd;
	fscanf(stdin, "%d", &na);
	fscanf(stdin, "%d", &nd);
	
	//int chevres[na+nd];
	int chevre;
	int truck[c+1];
	truck[0] = 1;
	for (int i=0; i<c;i++) {truck[i+1]=0;}
	for (int i=0; i<na+nd; i++) {
		fscanf(stdin, "%d", &chevre);
		if (chevre<=50 && c>=chevre) {
			for (int j=c; j>=chevre; j--) {
				if (truck[j-chevre]) {
					truck[j] = 1;
				}
			}
		}
		//printf("%d", chevres[i]);
	}
	
	for (int i=c; i>=0; i--) {
		if (truck[i]) {
			printf("%d\r\n", i);
			return 0;
		}
	}

	return 0;
}
