#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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
// READ ABOUT MPI SPARSE MATRIX VECTOR MULTIPLICATION

#define NODES 4039
#define EDGES 176468
double sqrt(double x);
int main()
{       printf("Program start\n");
	
	FILE *fp;
	int colindex, link,i,j=0,col,colmatch=0,localsum=0;
	//double a[4][4] = {{0}};
        int k;	
	
	double * val = (double*)calloc(EDGES, sizeof(double));
	int * rowind =(int*)calloc(EDGES, sizeof(int));
	
	int * colptr = (int*)calloc(NODES+1, sizeof(int)); //missing  values will be initialized to zero? no.of cols+1
	int co, index; //for normalizing the array of non zero elements
	
	double * prold = (double*)malloc(NODES*sizeof(double));; //malloc and initialize to 0.25
	double * prnew = (double*)calloc(NODES, sizeof(double));
	double * damp1 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.85
	double * damp2 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.15/NODES
	double * diff = (double*)calloc(NODES, sizeof(double));
	double err = 0.00001;
	double norm, norm_sq;
	int * sum = (int*)calloc(NODES, sizeof(int));

	for(i=0; i<NODES;i++) {
		prold[i] = 0.25;
		damp1[i] = 0.85;
		damp2[i] = 0.15/NODES;
	}
	printf("initialization complete\n");

//function start
	fp = fopen("rawdata1.dat", "r");
	for(i = 0; i<EDGES; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
//Do this if node numbers start with zero		
		colindex = colindex - 1;
		link = link - 1;
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
	colptr[j+1]= EDGES; //number of non zeros in the matrix
	fclose(fp);
	index = 0;
	for(i = 0; i<NODES; i++) { // This is to normalize the array of non zeros
		co = sum[i];
		for(j = index; j < index+co; j++) {
			val[j] = val[j]/co;
		}
		index += co;
	}
	// So much should go in the function
        printf("this is the val array\n");
	for(i = 0; i<EDGES; i++) {
		printf("%f\t", val[i]);
	}
//	printf("\n");
//	printf("this is the sum array\n");	
//	// sum array
//	for(j = 0; j< NODES; j++) {
//		printf("%d\t", sum[j]);
//	}
	printf("\n");
	printf("this is the rowind array\n");
	for(i=0;i<EDGES;i++) {
		printf("%d\t", rowind[i]);
	}
        printf("\nthis is the colptr array\n");
        for(i=0;i<NODES+1;i++) {
		printf("%d\t", colptr[i]);
	}

// val, rowind, colptr calculation complete... all the above should
// go to mpi file..



//checking matrix multiplication	
//*********Computing pagerank*******************/

	printf("\nComputing page rank...\n");
// Power iteration
// This can be enlosed in a function
// For each submatrices, if the right values of col, val
// rowind and prold are available at the processor,
// multiplication can be done successfully.
        do {
                memset(prnew, 0, NODES*sizeof(double));
		for(j = 0; j<NODES; j++) {
			printf("%f\t", prold[j]);
		}
		printf("\n");
		norm = 0.0;
		for(col = 0; col<NODES; col++) {
			for(j=colptr[col]; j<colptr[col+1]; j++) {
				prnew[rowind[j]] += val[j]*prold[col];
			}
		}
		for(i = 0; i<NODES; i++) {
			prnew[i] = prnew[i]*damp1[i]+damp2[i];
			printf("%f\t", prnew[i]);
		}
                printf("\n");
//norm calculation and vector copying from new to old
		norm_sq = 0.0;
		for(i=0; i<NODES;i++) {
			diff[i] = prnew[i] - prold[i];
			norm_sq += diff[i]*diff[i]; //l2 norm || prnew-prold ||
		//	printf("%f\n",fabs(diff[i]*diff[i]));
			prold[i] = prnew[i];
			//printf("%f\t", prold[i]);
		}
		//for(j=0; j < NODES; j++) {
		//	printf("%f\t", prold[j]);
	//	printf("%f\n", norm_sq);
	        norm = sqrt(norm_sq);
	}while(norm>err);
//print pagrank vector	
	for(j = 0; j< NODES; j++) {
		printf("%f\t",prnew[j]);
	}
	return 1;
}
