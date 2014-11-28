#include <stdio.h>

// serial sparse implementation of pagerank. four node graph
// R(t+1) = d M R(t) + (1-d)/N
// d - damp1, N = 4
// damp2 - (1-d)/N
// prnew <- R
// a <- M
// **************Compressed column storage technique**************//
// All non zero elements will be stored as val vector
// Row indices of these elements are in rowind vector
// Indices of those elements which start a new column
// are in colptr vector

main()
{
	FILE *fp;
	int colindex, link,i,j=0,col,colmatch=0,localsum=0;
	//double a[4][4] = {{0}};
	double val[9] = {0}; //there are 9 edges or 9 non zero elements
	int rowind[9] ={0};
	int colptr[5] = {0}; //missing  values will be initialized to zero? no.of cols+1
	int co, index; //for normalizing the array of non zero elements
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
		rowind[i] = link;
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
	index = 0;
	for(i = 0; i<4; i++) { // This is to normalize the array of non zeros
		co = sum[i];
		for(j = index; j < index+co; j++) {
			val[j] = val[j]/co;
		}
		index += co;
	}
        printf("this is the val array\n");
	for(i = 0; i<9; i++) {
		printf("%f\t", val[i]);
	}
	printf("\n");
	printf("this is the sum array\n");	
	// sum array
	for(j = 0; j< 4; j++) {
		printf("%d\t", sum[j]);
	}
	printf("\n");
	printf("this is the rowind array\n");
	for(i=0;i<9;i++) {
		printf("%d\t", rowind[i]);
	}
        printf("\nthis is the colptr array\n");
        for(i=0;i<5;i++) {
		printf("%d\t", colptr[i]);
	}
//checking matrix multiplication	
//*********Computing pagerank*******************//

	printf("\nComputing page rank...\n");
// Power iteration
        do {
		norm = 0.0;
		for(col = 0; col<4; col++) {
			for(j=colptr[col]; j<colptr[col+1]; j++) {
				prnew[rowind[j]] += val[j]*prold[col];
			}
		}
		for(i = 0; i<4; i++) {
			prnew[i] = prnew[i]*damp1[i]+damp2[i];
		}

//norm calculation and vector copying from new to old
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
