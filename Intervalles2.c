#include <stdio.h>

typedef struct {
	int pos;
	int event;
} PosEvent;

int main() {
	int n;
	scanf("%d", &n);
	PosEvent events[2*n];
	for (int i=0;i<n;i++) {
		scanf("%d %d", &events[2*i].pos, &events[2*i+1].pos);
		events[2*i].event = 1;
		events[2*i+1].event = -1;
	}
	
	PosEvent tmp;
	for (int i=0;i<2*n;i++) {
		for (int j=0;j<2*n-i-1;j++) {
			if (events[j].pos > events[j+1].pos) {
				tmp = events[j];
				events[j] = events[j+1];
				events[j+1] = tmp;
			}
		}
	}
	
	int depth = 0;
	int in = 0;
	int ni = 0;
	int inter[2*n];
	for (int i=0;i<2*n;i++) {
		depth += events[i].event;
		if (!in && depth != 0) {
			in = 1;
			inter[2*ni] = events[i].pos;
			ni += 1;
		}
		if (in && depth == 0) {
			if (i<2*n-1 && events[i+1].event == 1 && events[i+1].pos == events[i].pos) {
				continue;
			}
			in = 0;
			inter[2*(ni-1) + 1] = events[i].pos;

		}
		
	}
	printf("%d\r\n", ni);
	for (int i = 0;i<2*ni;i+=2){
		printf("%d %d\r\n", inter[i], inter[i+1]);
	}

	return 0;
}
