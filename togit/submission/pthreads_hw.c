#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
/*
#define NODES 4
#define EDGES 9
#define CORES 3
#define MASTER 0
double sqrt(double x);
void *mat_vec(void *);
double *val, *prold, *prnew, *damp1, *damp2, *diff;
int *rowind, *colptr, *sum;
pthread_mutex_t add_mutex;
pthread_mutex_t wait_mutex;
pthread_barrier_t our_barrier;
pthread_cond_t proceed_cv;
double norm, norm_sq,err = 0.000001;
int var_wait;
int main()
{       printf("Program start\n");
	pthread_t p_threads[CORES];
	pthread_mutex_init(&add_mutex, NULL);
	pthread_mutex_init(&wait_mutex, NULL);
	pthread_barrier_init(&our_barrier, NULL, CORES);
	pthread_cond_init(&proceed_cv, NULL);
	FILE *fp;
	int colindex, link,i,j=0,col,colmatch=0,localsum=0;
	long t;
	//double a[4][4] = {{0}};
        int k,rc;	
	
	val = (double*)calloc(EDGES, sizeof(double));
	rowind =(int*)calloc(EDGES, sizeof(int));
	
	colptr = (int*)calloc(NODES+1, sizeof(int)); //missing  values will be initialized to zero? no.of cols+1
	int co, index; //for normalizing the array of non zero elements
	
	prold = (double*)malloc(NODES*sizeof(double));; //malloc and initialize to 0.25
	prnew = (double*)calloc(NODES, sizeof(double));
	damp1 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.85
	damp2 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.15/NODES
	diff = (double*)calloc(NODES, sizeof(double));
	double err = 0.00001;
	double norm, norm_sq;
	sum = (int*)calloc(NODES, sizeof(int));

	for(i=0; i<NODES;i++) {
		prold[i] = 0.25;
		damp1[i] = 0.85;
		damp2[i] = 0.15/NODES;
	}
	printf("initialization complete\n");

	fp = fopen("sample.txt", "r");
	for(i = 0; i<EDGES; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
//Do this if node numbers start with zero		
//		colindex = colindex - 1;
//		link = link - 1;
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
// Thread creation	
	for(t=0; t<CORES; t++) {
		printf("\nIn main: creating thread %ld\n", t);
		pthread_create(&p_threads[t], NULL, mat_vec, (void*)t);
		if (rc) {
			printf("ERROR: return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for(t= 0; t<CORES; t++) {
		pthread_join(p_threads[t], NULL);
	}
//	printf("Printing pagerank vector\n");
//	for(j=0; j<NODES; j++) {
///		printf("%f\t", prnew[j]);
//	}
*/	/* Clean up and exit */
//	pthread_mutex_destroy(&add_mutex);
//	pthread_mutex_destroy(&wait_mutex);
//	pthread_cond_destroy(&proceed_cv);
/*	pthread_exit(NULL);
}

//checking matrix multiplication	
*********Computing pagerank*******************/

// Power iteration
// This can be enlosed in a function

/*void *mat_vec(void *rank) {
	long tid = (long)rank;
        printf("\n%ld REached thread function\n",tid);
	int i,j,col,local_col,my_first_col,my_last_col;
//	double * localpr = (double*)calloc(NODES, sizeof(double));
	printf("my thread id is%ld\n", tid);
	if (tid==0) {
		local_col = NODES/CORES + NODES%CORES;
		my_first_col = 0;
		my_last_col = my_first_col + local_col - 1;
	}
	else {
		local_col = NODES/CORES;
		my_first_col = tid*local_col + NODES%CORES;
		my_last_col = my_first_col + local_col - 1;
		printf("this is %d\t %d\n", tid, my_last_col);
	}*/
/*	
// do starts here
        printf("reached do\n");
	printf("%d\t%d\n", my_first_col, my_last_col);
	do {
		for(col=my_first_col; col<=my_last_col; i++) {
			for(j = colptr[col]; j<colptr[col+1]; j++) {
				localpr[rowind[j]] += val[j]*prold[col];
				localpr[rowind[j]] = localpr[rowind[j]]*damp1[rowind[j]] + damp2[rowind[j]];
			}
		}
		for(i=0; i<NODES; i++) {
		        printf("%d\n", i);
			if (localpr[i]!=0.0) {
			        printf("%d\n",i);
				pthread_mutex_lock(&add_mutex);
				prnew[i] += localpr[i];
				pthread_mutex_unlock(&add_mutex);
			}
		}
        // barrier and conditional variable
        // MASTER thread does the comparison during which other threads sleep
        //prnew <- 0
	        printf("REached barrier\n");
	        pthread_barrier_wait(&our_barrier); //makes sure all threads have added their contributions to prnew
		if (rank==MASTER) {
			var_wait = 0;
			norm_sq = 0.0;
			for(i=0; i<NODES; i++) {
				diff[i] = prnew[i] - prold[i];
				norm_sq += diff[i]*diff[i];
				prold[i] = prnew[i];
			}
			norm = sqrt(norm_sq);
			memset(prnew, 0, NODES*sizeof(double));// for next iteration
			memset(localpr, 0, NODES*sizeof(double));
			var_wait = 1;
			if(var_wait) {
				pthread_cond_signal(&proceed_cv);
			}
		}
		else {
			pthread_mutex_lock(&wait_mutex);
			while(!var_wait) {
				pthread_cond_wait(&proceed_cv,&wait_mutex);
			}
			pthread_mutex_unlock(&wait_mutex);
			memset(localpr, 0, NODES*sizeof(double));
		}
	}while(norm>err);
	
	pthread_exit(NULL);
}
*/
#define CORES 2
#define EDGES 9
#define NODES 4
void *mat_vec(void *);
double *val, *prold, *prnew, *damp1, *damp2, *diff;
int *rowind, *colptr, *sum;
pthread_mutex_t add_mutex;
pthread_mutex_t wait_mutex;
pthread_barrier_t our_barrier;
pthread_cond_t proceed_cv;
double norm, norm_sq, err = 0.000001;
int var_wait;
int co=0;
int main() {
        printf("Program start\n");
	pthread_t p_threads[CORES];
	pthread_mutex_init(&add_mutex, NULL);
	pthread_mutex_init(&wait_mutex, NULL);
	pthread_barrier_init(&our_barrier, NULL, CORES);
	pthread_cond_init(&proceed_cv, NULL);
	FILE *fp;
	int colindex, link,i,j=0,col,colmatch=0,localsum=0;
	long t;
	val = (double*)calloc(EDGES, sizeof(double));
	rowind =(int*)calloc(EDGES, sizeof(int));	
	
	colptr = (int*)calloc(NODES+1, sizeof(int)); //missing  values will be initialized to zero? no.of cols+1
	int co, index; //for normalizing the array of non zero elements
	
	prold = (double*)malloc(NODES*sizeof(double));; //malloc and initialize to 0.25
//        double prold[4] = {0.409375,0.090625, 0.303125, 0.196875};
        prnew = (double*)calloc(NODES, sizeof(double));
	damp1 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.85
	damp2 = (double*)malloc(NODES*sizeof(double)); //malloc and initialize to 0.15/NODES
	diff = (double*)calloc(NODES, sizeof(double));
	double err = 0.00001;
	double norm, norm_sq;
	sum = (int*)calloc(NODES, sizeof(int));

	for(i=0; i<NODES;i++) {
//		prold[i] = 0.25;
		damp1[i] = 0.85;
		damp2[i] = 0.15/NODES;
	}
	printf("initialization complete\n");

	fp = fopen("sample.txt", "r");
	for(i = 0; i<EDGES; i++) {
		fscanf(fp, "%d %d", &colindex, &link);
//Do this if node numbers start with zero		
//		colindex = colindex - 1;
//		link = link - 1;
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
/*        printf("this is the val array\n");
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
*/
        for(t=0; t<CORES; t++) {
		pthread_create(&p_threads[t], NULL, mat_vec, (void*)t);
	}
	for(t=0;t<CORES; t++) {
		pthread_join(p_threads[t], NULL);
	}

	printf("Printing final pagerank vector\n");
	for(j=0; j<NODES; j++) {
		printf("%f\t", damp1[j]*prold[j]+damp2[j]);
	}
	/* Clean up and exit */
	pthread_mutex_destroy(&add_mutex);
	pthread_mutex_destroy(&wait_mutex);
	pthread_cond_destroy(&proceed_cv);
	pthread_exit(NULL);
	free(prold);
	free(prnew);
	free(diff);
}
void *mat_vec(void *rank) {
	long tid = (long)rank;
	printf("Reached thread function\n");
	printf("my thread id is%ld\n", tid);
	int ii,jj,col,local_col,my_first_col,my_last_col;
	double * localpr = (double*)calloc(NODES, sizeof(double));
	if (tid==0) {
		local_col = NODES/CORES + NODES%CORES;
		my_first_col = 0;
		my_last_col = my_first_col + local_col - 1;
		printf("this is %ld\t %d\t %d\n", tid,my_first_col, my_last_col);
	}
	else {
		local_col = NODES/CORES;
		my_first_col = tid*local_col + NODES%CORES;
		my_last_col = my_first_col + local_col - 1;
		printf("this is %ld\t %d\t %d\n", tid, my_first_col, my_last_col);
	}

// do starts here
//	printf("%ld\t myfirstcol%d\t mylastcol%d\n", tid, my_first_col, my_last_col);
	do {
//	        if (tid==0) {
//			memset(prnew, 0, NODES*sizeof(double));// for next iteration
//			printf(" i am %ld\t at top of do loop co is%d\n", tid,co);
//		}
		memset(localpr,0, NODES*sizeof(double)); 
		for(col=my_first_col; col<=my_last_col; col++) {
			for(jj = colptr[col]; jj<colptr[col+1]; jj++) {
				localpr[rowind[jj]] += val[jj]*prold[col];
				
			}
		}
		for(ii=0; ii<NODES; ii++) {
		        printf("this is %ld\n", tid);
		        printf("%f\t", localpr[ii]);
		}

		for(ii=0; ii<NODES; ii++){
	                pthread_mutex_lock(&add_mutex);
		        prnew[ii] += localpr[ii];
		        pthread_mutex_unlock(&add_mutex);
		}
        // barrier and conditional variable
        // MASTER thread does the comparison during which other threads sleep
        //prnew <- 0
	        pthread_barrier_wait(&our_barrier); //makes sure all threads have added their contributions to prnewi
		printf("passed barrier  threadid is%ld\n", tid);
		if (tid==0) {
			var_wait = 0;
			norm_sq = 0.0;
			for(ii=0; ii<NODES; ii++) {
				diff[ii] = prnew[ii] - prold[ii];
				norm_sq += diff[ii]*diff[ii];
				prold[ii] = prnew[ii];
			}
			norm = sqrt(norm_sq);
			memset(prnew, 0, NODES*sizeof(double));// for next iteration
			var_wait = 1;
			printf("this is the first element %f\n", prold[0]);
			if(var_wait) {
				pthread_cond_signal(&proceed_cv);
			}
		}
		else{   
			pthread_mutex_lock(&wait_mutex);
			while(!var_wait) {
			        printf("WAITING\n");
				pthread_cond_wait(&proceed_cv,&wait_mutex);
			}
			pthread_mutex_unlock(&wait_mutex);
		}
		printf("i am %ld\t at the endreached end of do loop co is %d\n",tid,co);
	}while(0);
	pthread_exit(NULL);
}
