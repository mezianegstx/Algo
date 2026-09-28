#include <stdio.h>
int main() {
int n;
scanf("%d", &n);
long int result = 1;
for (int i=1; i<=n;i++) {
	result *=i;
}

printf("%ld\r\n", result);
return 0;
}
