```objective-c
MPI_Scatterv(val, sendcnts, displs, MPI_DOUBLE, rec_val, sendcnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);//non-zero elements
printf("first scatterv completed\n");
MPI_Scatter(pcols, 1, MPI_INT, &rec_col, 1, MPI_INT, 0, MPI_COMM_WORLD);//number of columns in each processor
printf("scatter completed\n");
MPI_Scatterv(rowind, sendcnts, displs, MPI_INT, rec_row, sendcnts[rank], MPI_INT, 0, MPI_COMM_WORLD);//rowindices
printf("second scatterv completed\n");
```

These are examples of scatter and scatterv communication operations commonly used in MPI. Let us break this down.

```objective-c
MPI_Scatterv(val, sendcnts, displs, MPI_DOUBLE, rec_val, sendcnts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
```

val, sendcnts, displs are all arrays. MPI_DOUBLE is the type of elements being sent and received.
What this means is from process with rank 0, scatter elements in val, such that process with rank say 1, will receive sendcnts[1], with rank 2 will sendcnts[2] number of elements.. and so on. The received elements are stored in a local rec_val array. 'displs[i]' is the index of sendcnts from which the next set of elements are scattered.

```objective-c
displs[i] = displs[i-1] + sendcnts[i-1]
```
