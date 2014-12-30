// parallel implementation of pagerank computation
// R(t+1) = d M R(t) + (1-d)/N
// d - damp1, N = 4
// damp2 - (1-d)/N
// prnew <- R
// **************Compressed column storage technique**************//
// All non zero elements will be stored as val vector
// Row indices of these elements are in rowind vector
// Indices of those elements which start a new column
// are in colptr vector
// READ ABOUT MPI SPARSE MATRIX VECTOR MULTIPLICATION
// mpicc pagerank_mpi.c -o parallel -lm
// mpirun -machinefile machines -np 2 parallel
// This file is for data1.dat

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define NODES 4039
#define EDGES 176468
#define MASTER 0

main()
{       printf("Program start\n");
	
	FILE *fp;
	clock_t begin,end;
	double time_spent;
	int NPROC, rank, root, colindex, link,i,j=0, k, col,colmatch=0,localsum=0;	
        int newlines = 0, linenum = 5;
	char ch;
	char line[21];
	double * val = (double*)calloc(EDGES, sizeof(double));
//        double val[EDGES];
	int * rowind =(int*)calloc(EDGES, sizeof(int));
//	int rowind[EDGES];
	int *sendcnts;
	int *displs;
	int * colptr = (int*)calloc(NODES+1, sizeof(int)); //missing  values will be initialized to zero? no.of cols+1
	int co, index; //for normalizing the array of non zero elements
	
	double * pr = (double*)malloc(NODES*sizeof(double));; //malloc and initialize to 0.25
	double * prnew = (double*)calloc(NODES, sizeof(double));
	double * damp1 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.85
	double * damp2 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.15/NODES
	double * diff = (double*)calloc(NODES, sizeof(double));
        double * sum = (double*)calloc(NODES, sizeof(double)); //this is for the column vectors
	double * rec_val = (double*)malloc(100000*sizeof(double));
        double * rec_pr = (double*)malloc(100000*sizeof(double));
//        double rec_val[4000], rec_pr[4000]; //these receive the scattered vector parts in the processes
	double err = 0.00001;
	double norm, norm_sq;
	int * readsum = (int*)calloc(NODES, sizeof(int));
        int rec_col;
	int * rec_row = (int*)malloc(100000*sizeof(int));
        MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &NPROC);
	printf("Number of processes %d\n", NPROC);
        sendcnts= malloc(sizeof(int)*NPROC);
        displs = malloc(sizeof(int)*NPROC);
        int * pcols = (int*)malloc(NPROC*sizeof(int)); //No.of columns to each processor
//	int pcols[NPROC];
	int * displs_pr = (int*)malloc(NPROC*sizeof(int));// displacement for pagerank vector scatter

	for(i=0; i<NODES;i++) {
		pr[i] = 0.25;
		damp1[i] = 0.85;
		damp2[i] = 0.15/NODES;
	}
	printf("initialization complete\n");
	printf(" I am rank %d\n", rank);
//1. Populate val, rowind, colptr
//2. Calculate number of columns to each process
//3. Number of pagerank vector elements to each process
//4. Distribution of non zero elements to corresponding processes

        fp = fopen("data1.dat", "r");
	while((ch=getc(fp)) != EOF) {
	        if(ch == '\n') {
		        newlines += 1;
			if(newlines == linenum - 1) {
			        break;
			}
		}
	}	
	for(i = 0; i<EDGES; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
//Uncomment the next two lines if node numbers does not start with zero		
         	colindex = colindex - 1;
		link = link - 1;
		rowind[i] = link;
		if(colmatch==colindex) {
			localsum += 1;
		}
		else {
			readsum[j] = localsum;
			colptr[j+1] = colptr[j] + localsum; //index of val where new column starts
			localsum = 1; //new localsum
			j += 1;
			colmatch = colindex; //new column
		}
		val[i] = 1.0;
	}
	readsum[j] = localsum; //for the last column
	colptr[j+1]= EDGES; //number of non zeros in the matrix
	fclose(fp);
	index = 0;
	for(i = 0; i<NODES; i++) { // This is to normalize the array of non zeros
		co = readsum[i];
		for(j = index; j < index+co; j++) {
			val[j] = val[j]/co;
		}
		index += co;
	}
	printf("val, rowind and colptr have been populated\n");
// val, rowind, colptr calculation complete... all the above should
// go to mpi file..

// Calculate number of columns to each process

        for(i=0; i<NPROC; i++) {
		if(i==0) {
			pcols[i] = NODES/NPROC + NODES%NPROC;
			displs_pr[i] = 0;
		}else {
			pcols[i] = NODES/NPROC;
			displs_pr[i] = pcols[i-1] + displs_pr[i-1];
		}
	}
// Calculating sendcnts and displs
        j = 0;
	for(i=0; i<NPROC; i++) {
		j = j + pcols[i];
		k = j - pcols[i];
		sendcnts[i] = colptr[j] - colptr[k];
		if (i==0) {
			displs[i] = 0;
		}else
			displs[i] = sendcnts[i-1] + displs[i-1];
	}
	if(rank==MASTER) {
	        printf("This is MASTER\n");
		
	        printf("\nsendcnts\n");
		for(i=0;i<NPROC;i++) {
		         printf("%d\t", sendcnts[i]);
	        }
	        printf("\ndispls\n");
	        for(i=0;i<NPROC;i++) {
		         printf("%d\t", displs[i]);
	        }	
	        printf("\nval\n");
//	        for(i=0;i<EDGES; i++) {
//	                 printf("%f\t", val[i]);
//	        }
	}
//	double val[] = {0.25,0.25,0.25,0.25,0.5,0.5,1.0,0.5,0.5};
//	int sendcnts[] = {6,3};
//	int displs[] = {0,6};
	MPI_Scatterv(val, sendcnts, displs, MPI_DOUBLE, rec_val, sendcnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);//non-zero elements
	printf("first scatterv completed\n");
	MPI_Scatter(pcols, 1, MPI_INT, &rec_col, 1, MPI_INT, 0, MPI_COMM_WORLD);//number of columns in each processor
	printf("scatter completed\n");
	MPI_Scatterv(rowind, sendcnts, displs, MPI_INT, rec_row, sendcnts[rank], MPI_INT, 0, MPI_COMM_WORLD);//rowindices 	
	printf("second scatterv completed\n");
	double *vec[rec_col];// Initializing vector columns
	for(i=0; i<rec_col; i++) {
		vec[i] = (double *)calloc(NODES,sizeof(double));
	}
	k=0;
	for(j=0;j<rec_col;j++) {// Splitting rec_val to columnvectors
		for(i=0;i<NODES;i++) {
			if(i==rec_row[k]) {
				vec[j][i] = rec_val[k];//these vectors don't change with iterations
				k+=1;
			}
		}
	}
	begin = clock();
	do //Only MASTER contains updated prnew and pr.. each process has its own sum
	{  // norm is calculated by MASTER but is distributed to all for loop continuity
		memset(sum, 0, NODES*sizeof(double));
		if(rank == MASTER) {
		        memset(prnew, 0, NODES*sizeof(double));
		}	
		norm = 0.0;
		//Scatter and multiply
                MPI_Scatterv(pr, pcols, displs_pr, MPI_DOUBLE, rec_pr, pcols[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
		for(i=0;i<NODES;i++) {
			for(j=0;j<rec_col;j++) {
				sum[i] += vec[j][i]*rec_pr[j];// sum of column multiplications
			}
		}
		MPI_Reduce(sum, prnew, NODES, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);//vector sums into master
		// Normalizing
		if(rank == MASTER) {
		        for(i=0;i<NODES;i++) {
			        prnew[i]= prnew[i]*damp1[i] + damp2[i];
			}
			norm_sq = 0.0;
			for(i=0; i<NODES; i++) {// for norm calculation
			        diff[i] = prnew[i] - pr[i];
				norm_sq += diff[i]*diff[i];
				pr[i] = prnew[i];
			}
			norm = sqrt(norm_sq);
		}
		MPI_Bcast(&norm, 1, MPI_DOUBLE, MASTER, MPI_COMM_WORLD);
		//if(rank==MASTER) {
		//printf("Reduced page rank vector:\n");
		//for(i=0;i<NODES;i++) {
		//	printf("%f\t", prnew[i]);
		//}
	}while(norm>err);
	end = clock();
	time_spent = (double)(end-begin)/CLOCKS_PER_SEC;
	if(rank==MASTER) {
	        printf("\npagerank vector first ten elements\n");
		for(i = 0; i<10; i++) {
		        printf("%f\t", prnew[i]);
		}
		printf("\n");
		printf("Time taken for power iteration solution %fseconds\n",time_spent);
	}
	MPI_Finalize();

}
