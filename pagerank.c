#include <stdio.h>
main()
{
	FILE *fp;
	int colindex, link,i,j;
	int a[4][4] = { {0}, {0}, {0}, {0}};
	fp = fopen("sample.txt", "r");
	for(i = 0; i<9; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
		a[link][colindex] = 1;
	}
	fclose(fp);
	for(i = 0; i <4; i++) {
		printf("\n");
		for(j = 0; j<4; j++) {
			printf("%d\t",a[i][j]);
		}
	}
}
