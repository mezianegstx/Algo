#include <stdio.h>

int main(void) {
	int W;
	scanf("%d", &W);

	int n;
	scanf("%d", &n);
	
	int nWeight;
	char chType;
	
	int nZ = 0;
	int pZ[n];

	int nR = 0;
	int pR[n];
	
	int weights[201] = {0};

	for (int i=0; i<n; i++) {
		scanf(" %c %d", &chType, &nWeight);
		if (chType == 'Z') {
			pZ[nZ++] = nWeight;
			for (int j=0; j<nR; j++) {
				weights[pR[j]+nWeight] = 1;
			}
		} else {
			pR[nR++] = nWeight;
			for (int j=0; j<nZ; j++) {
                                weights[pZ[j]+nWeight] = 1;
                        }
		}
	}

	//for (int i=0;i<201;i++) {printf("%d %d\n", i, weights[i]);}

	int arr[W+1];
	//int arr2[nWeight+1];
	arr[0] = 1;
	//int arr2[0] = 1;
	for (int i=0;i<W;i++) {
		arr[i+1] = 0;
		//arr2[i+1] = 0;
	}

	


	for (int w=1; w<=W; w++) {
		for (int i=0; i<201; i++) {
			if (weights[i] && w>=i && arr[w-i]) {
		//		printf("OK : %d", i);
				arr[w] = 1;
			}
		}
		//printf("%d\n", arr[w]);
	}

	if (arr[W]==1) {
		printf("OUI\r\n");
	} else {
		printf("NON\r\n");
	}
	
	return 0;
}
