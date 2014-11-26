#include <stdio.h>

// serial implementation of pagerank. four node graph
main()
{
	FILE *fp;
	int colindex, link,i,j=0,colmatch=0,localsum=0;
	double a[4][4] = {{0}};
	double prold[4] = {0.25,0.25,0.25,0.25};
	double prnew[4] = {0.0,0.0,0.0,0.0};
	double diff[4] = {0.0,0.0,0.0,0.0};
	double err = 0.0001;
	double damp1[4] = { 0.85,0.85,0.85,0.85 };
	double damp2[4] = { 0.15,0.15,0.15,0.15};
	double norm;
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
			printf("%f\t",a[i][j]);
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
			printf("%f\t", a[i][j]);
		}
	}
	printf("\nComputing page rank...\n");
// Power iteration
        do {
		norm = 0.0;
		for(i = 0; i<4; i++) {
			for(j=0; j<4; j++) {
				prnew[i] += a[i][j]*prold[j];
			}
         	}
		//copying
		for(i=0; i<4;i++) {
			diff[i] = prnew[i] - prold[i];
			norm += diff[i]; //l1 norm || prnew-prold ||
			prold[i] = prnew[i];
		}
	}while(abs(norm)>err);
       //print pagrank vector	
	for(j = 0; j< 4; j++) {
		printf("%f\t", damp2[j] + damp1[j]*prnew[j]);
	}
}
