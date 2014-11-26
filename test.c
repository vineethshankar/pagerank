#include <stdio.h>
main()
{
	FILE *fp;
	int colindex, link,i,j=0,colmatch=0,localsum=0;
	int a[4][4] = {{0}};
	int sum[4] = { 0,0,0,0};
	fp = fopen("sample.txt", "r");
	for(i = 0; i<9; i++) {
		fscanf(fp, "%d %d", &colindex, &link);	
	//	if(colindex==3) {
	//		printf("%d\n", localsum);
		if(colmatch==colindex) {
			localsum += 1;
		}
		else {
			sum[j] = localsum;
			localsum = 1; //new localsum
			j += 1;
			colmatch = colindex; //new column
		}
		a[link][colindex] = 1.0;
	}
	sum[j] = localsum;
	fclose(fp);
	for(i = 0; i<4; i++) {
		printf("\n");
		for(j = 0; j<4; j++) {
			printf("%d\t",a[i][j]);
		}
	}
	printf("\n");	
	// sum array
	for(j = 0; j< 4; j++) {
		printf("%d\t", sum[j]);
	}
	printf("\n");
	for(j=0;j<4;j++) {
		for(i=0;i<4;i++) {
			a[i][j] = a[i][j]/sum[j];
		}
	}
        printf("printing final array\n");  
	for(i = 0; i<4; i++) {
		printf("\n");
		for(j=0; j<4; j++) {
			printf("%d\t", a[i][j]);
		}
	}


}
