/**
 * Array structure:
 * [0]:
 *      0 - initial message
 *      1 - delivery approve message
 * [1]: message sender
 * [2]: message reciever
 * [3]: random number - message
 */

#include "mpi.h"
#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cctype>
#include <random>

#define PROCESS_COUNT 5
#define MASTER 0

using u32 = uint_least32_t; 
using engine = std::mt19937;

/**
 * Generate random number in the specified range (inclusive)
 */
int getRandNumb(int min, int max) {
    std::random_device os_seed;
    const u32 seed = os_seed();

    engine generator(seed);
    std::uniform_int_distribution<u32> distribute(min, max);

    return distribute(generator);
}

/**
 * Get number of  process to sent
 * To avoid sending a request to yourself
 */
int getRandreciever(int currentProcess) {
    int res = getRandNumb(1, PROCESS_COUNT - 1);
    while (res == currentProcess) {
        res = getRandNumb(1, PROCESS_COUNT - 1);
    }

    return res;
}

int main(int argc, char *argv[]) {
    int ProcNum;
    int ProcRank;
    int RecvRank;
    MPI_Status Status;
    MPI_Request request;
    
    // init parallel block
    MPI_Init(&argc, &argv);

    // declare size of processes (group id, group size(return))
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);

    // define process rank in groud (group id, rank(return))
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    if(ProcRank == MASTER) {
        int msgGet[4];
        int msgSent[4];

        for (int i = 1; i < PROCESS_COUNT; ++i) {
            MPI_Recv(msgGet, 4, MPI_INT, i, 0, MPI_COMM_WORLD, &Status);
            msgSent[1] = msgGet[1];
            msgSent[2] = msgGet[2];
            msgSent[3] = msgGet[3];
            if (msgGet[0] == 0) {
                // recieve initial message
                msgSent[0] = 1;
                std::cout << std::flush << "MASTER process recieve " << msgGet[3] << " hash from " << msgGet[1] << " process to " << msgGet[2] << " process" << std::endl;
                MPI_Send(msgSent, 4, MPI_INT, msgGet[2], 0, MPI_COMM_WORLD);
            } else if (msgGet[0] == 1) {
                // send approve message
                msgSent[0] = 0;
                std::cout << std::flush << "MASTER sent approve to the " << msgSent[1] << " process" << std::endl;
                MPI_Isend(msgSent, 4, MPI_INT, msgGet[1], 0, MPI_COMM_WORLD, &request);
            }
        }
    } else {
        int msgSent[4];
        int recieverRank = getRandreciever(ProcRank);
        int hash = getRandNumb(1000, 9999);
        msgSent[0] = 0;
        msgSent[1] = ProcRank;
        msgSent[2] = recieverRank;
        msgSent[3] = hash;
        MPI_Send(msgSent, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
        std::cout << std::flush << "Process " << ProcRank << " SENT hash " << hash << " to " << recieverRank << " process" << std::endl;

        int msgApproveGet[4];
        MPI_Irecv(msgApproveGet, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
        if (msgApproveGet[0] == 1) {
            // get initial message and send approve
            std::cout << std::flush << "Process " << ProcRank << " GET hash " << msgApproveGet[3] << " from " << msgApproveGet[1] << " process" << std::endl;
            MPI_Send(msgSent, 4, MPI_INT, 0, 0, MPI_COMM_WORLD);
        } else if (msgApproveGet[0] == 0) {
            std::cout << std::flush << "Process " << ProcRank << " APPROVE " << msgApproveGet[3] << " hash" << std::endl;
        }
    }

    // finish parallel block
    MPI_Finalize();

    return 0;
}
