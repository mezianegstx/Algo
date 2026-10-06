#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	float x;
	float y;
	float r;
} Circle;

int main(void) {
	int n;
	scanf("%d", &n);
	//float x, y, r;
	Circle circles[n];
	int count = 0;
	for (int i = 0; i<n; i++) {
		Circle * c= malloc(sizeof(Circle));
		scanf("%f %f %f", &c->x, &c->y, &c->r);
		for (int j=0; j<i; j++) {
			if (sqrt((c->x - circles[j].x)*(c->x - circles[j].x) + (c->y - circles[j].y)*(c->y - circles[j].y)) < c->r + circles[j].r) {
				count++;
			}
		}
		circles[i] = *c;
	}
	printf("%d\n", count);
	return 0;
}
