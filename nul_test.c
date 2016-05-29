#include <stdio.h>

int main()
{
	char arr[5];
	arr[0] = ' ';
	arr[1] = '\n';
	arr[2] = '\0';
	arr[3] = 'X';
	arr[4] = 'X';

	int i;
	for (i = 0; arr[i] != '\0'; i++) {
		printf("'%c'\t%d\n",arr[i],(int)arr[i]);
	}


	return 0;
}
