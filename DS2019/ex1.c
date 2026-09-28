#include <stdio.h>
#include <math.h>

float dist(float xa, float ya, float xb, float yb) {
	return (float)sqrt((double)((xb-xa)*(xb-xa) + (yb-ya)*(yb-ya)));
}

int main(void) {
	float x, y;
	scanf("%f %f", &x, &y);
	float R;
	scanf("%f", &R);

	int n;
	scanf("%d", &n);
	
	//float valides[2n];
	
	float xb, yb;
	int count=0;
	for (int i; i<n;i++) {
		scanf("%f %f", &xb, &yb);
		if (dist(x, y, xb, yb) < R) {
			//valides[count++] = xb;
			//valides[count++] = yb;
			count++;
		}
		
	}

	printf("%d\r\n", count);
	
	return 0;
}
