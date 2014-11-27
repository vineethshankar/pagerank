#include <stdio.h>

// serial sparse implementation of pagerank. four node graph
// R(t+1) = d M R(t) + (1-d)/N
// d - damp1, N = 4
// damp2 - (1-d)/N
// prnew <- R
// a <- M
// **************Compressed row storage technique**************//
// All non zero elements will be stored as val vector
// Column indices of these elements are in colind vector
// Indices of those elements which start a new row
// are in rowptr vector

main()
{
	FILE *fp;
	int colindex, link,i,j=0,colmatch=0,localsum=0;
	//double a[4][4] = {{0}};
	double val[9] = {0}; //there are 9 edges or 9 non zero elements
	int rowind[9] ={0};
	int colptr[10] = {1}; //missing  values will be initialized to zero?
	double prold[4] = {0.25,0.25,0.25,0.25};
	double prnew[4] = {0.0,0.0,0.0,0.0};
	double diff[4] = {0.0,0.0,0.0,0.0};
	double err = 0.0001;
	double damp1[4] = { 0.85,0.85,0.85,0.85 };
	double damp2[4] = { 0.0375,0.0375,0.0375,0.0375 };
	double norm;
	int sum[4] = { 0,0,0,0};
	fp = fopen("sample.txt", "r");
	for(i = 0; i<9; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
		rowind[i] = link + 1;
		if(colmatch==colindex) {
			localsum += 1;
		}
		else {
			sum[j] = localsum;
			colptr[j+1] = colptr[j] + localsum; //index of val where new column starts
			localsum = 1; //new localsum
			j += 1;
			colmatch = colindex; //new column
		}
		val[i] = 1.0;
	}
	sum[j] = localsum; //for the last column
	colptr[j+1]= 9; //number of non zeros in the matrix
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
			prnew[i] = prnew[i]*damp1[i]+damp2[i];
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
		printf("%f\t",prnew[j]);
	}
}
