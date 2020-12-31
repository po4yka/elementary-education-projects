#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv) {
   int rank, size, prev, next;
   int buf[2];
   MPI_Request reqs[2];
   MPI_Status stats[2];

   MPI_Init(&argc,&argv);
   double start = MPI_Wtime();

   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   prev = rank - 1;
   next = rank + 1;

   if(rank == 0) prev = size - 1;
   if(rank == size - 1) next = 0;

   // sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status
   MPI_Sendrecv(&rank, 1, MPI_INT, next, 5, &buf[0], 1, MPI_INT, prev, 5, MPI_COMM_WORLD, &stats[0]);
   printf("process %d got '%d', send '%d' to %d\n", rank, buf[0], rank, next);
   
   double end = MPI_Wtime();
   MPI_Finalize();
   
   printf("The process took %f seconds to run.\n", end - start);

   return 0;
}