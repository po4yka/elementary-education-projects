#include <stdio.h>
#include "mpi.h"

int main(int argc,char * argv[]) {
	int ierr, rank, size, i, n, nmax, lmax, NTIMES = 10;
	//const int NMAX = 1000000;
  	const int NMAX = 1000;
	double time_start,time,bandwidth,max;
	float a[NMAX*8]; // equal to real*8  a[NTIMES] at fortran
	struct MPI_Status status;
  	ierr = MPI_Init(&argc,&argv);
	// ierr=MPI_Comm_size(MPI_COMM_WORLD, &size);
	ierr = MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	// time_start=MPI_Wtime();
	n = 0;
	nmax = lmax = 0;
	while (n <= NMAX) {
		time_start=MPI_Wtime();
		for (i=0; i < NTIMES; ++i) {
 			if (rank == 0) { 
				ierr = MPI_Send(&a,8*n,MPI_FLOAT, 1, 1, MPI_COMM_WORLD) ;
 				ierr = MPI_Recv(&a, 8*n, MPI_FLOAT, 1, 1, MPI_COMM_WORLD, &status) ;
			} else if (rank == 1) {
				ierr=MPI_Recv(&a, 8*n, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status) ;
				ierr=MPI_Send(&a, 8*n, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
            } 
        }

		time = (MPI_Wtime() - time_start) / (2 * NTIMES); // this is time for one way transac-tion
		bandwidth = ((double)8 * n * sizeof(float))/ (1024 * 1024) / time;
		if (max < bandwidth) {
            max=bandwidth;
			lmax = 8 * n * sizeof(float);
        } 
		if (rank == 0) {
			if (!n) {
                printf("Latency = %10.4f seconds\n", time);
            } else {
                    printf("%li bytes sent, bandwidth = %10.4f MB/s\n", 8 * n * sizeof(float), bandwidth);
                    // printf("(%li; %10.4f)\n", 8*n*sizeof (float), bandwidth);
                }
			} 
		if (n == 0) {
            n = 1;
        } else {
            n *= 2;
        }
	}
	// Finally print maximum bandwidth
	if (rank==0) {
        printf("Max bandwidth = %10.4f MB/s, length = %i bytes\n", max, lmax);
    }
	ierr = MPI_Finalize();

	return 0; 
}
