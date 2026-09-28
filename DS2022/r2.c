#include <stdio.h>

int main(void) {
	char mot1[100];
	char mot2[100];
	scanf("%99s\n%99s", mot1, mot2);
	int i = 0;
	while (mot1[i]!='\0' && mot1[i] == mot2[i]) {i++;}
	if (mot1[i]==mot2[i]) {printf("n\r\n"); return 0;}

	i=0;
	char alpha[26];
	for (i=0; i<26;i++) {
		alpha[i]=0;
	}

	i=0;
	while (mot1[i] != '\0') {
		alpha[mot1[i++] - 'a'] += 1;
	}
	i=0;
	while (mot2[i]!='\0') {
		alpha[mot1[i++] - 'a'] -= 1;
	}

	for (i=0; i<26;i++) {
		if (alpha[i]) {
			printf("n\r\n");
			return 0;
		}
	}
	
	printf("o\r\n");

	return 0;
}
