#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define TRIALS 20
#define ARRAY_SIZE 100

#define FULL_PRINT
// #define TEST_1

int main(int argc, char *argv[]) {
    int myid, numprocs;
    double startwtime, endwtime;
    int namelen;
    unsigned long long* numbers = new unsigned long long[ARRAY_SIZE];
    unsigned long long i, j, sum, part_sum;
    unsigned long long startIndex, endIndex;
    int s, s0;
    double totalTime;

    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    #ifdef FULL_PRINT
    fprintf(stderr, "Process %d on %s\n", myid, processor_name);
    #endif

    fflush(stderr);

    for (i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = i;
    }

    if (myid == 0) {
        s = (int) floor(ARRAY_SIZE / numprocs);
        s0 = s + ARRAY_SIZE % numprocs;
        
        //printf("s=%d , s0= %d\n", s, s0);
    }

    MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&s0, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    startIndex = s0 + (myid - 1) * s;
    endIndex = startIndex + s;
    totalTime = 0;
    
    for (j = 1; j <= TRIALS; j++) {
        if (myid == 0) {
            startwtime = MPI_Wtime();
        }

        sum = 0;
        part_sum = 0;

        if (myid == 0) { // master
            // compute sum of master's numbers
            for (i = 0; i < s0; i++) {
                part_sum += numbers[i];
            }
        } else {
            for (i = startIndex; i < endIndex; i++) {
                part_sum += numbers[i];
            }
        }

        MPI_Reduce(&part_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        
        if (myid == 0) {
            double runTime;
            endwtime = MPI_Wtime();
            runTime = endwtime - startwtime;

            #ifdef FULL_PRINT
            printf("Trial %llu : Execution time (sec) = %f\n", j, runTime);
            printf("Sum = %llu \n", sum);
            #endif

            totalTime += runTime;
        }
    } // end for

    if (myid == 0) {
        #ifdef FULL_PRINT
        printf("-------------------------------------------\n");
        printf("Total time: %f (sec) for array with %d elements\n", totalTime, ARRAY_SIZE);
        printf("Average time for %d trials with %d processes: %f (sec)\n", TRIALS, numprocs, totalTime / TRIALS);
        #endif
        #ifdef TEST_1
        printf("%d %f\n", numprocs, totalTime);
        #endif
    }

    MPI_Finalize();
}