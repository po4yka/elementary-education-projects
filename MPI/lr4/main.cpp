#include "mpi.h"
#include <iostream>
#include <cctype>
#include <random>
#include <vector>

// for random
using u32 = uint_least32_t; 
using engine = std::mt19937;

int main(int ac,char **av)
{
    MPI_Init(&ac, &av);

    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size == 0) {
        std::cout << "Program has no sence with 0 processes" << std::endl;
        return 0;
    }

    // random generation
    std::random_device os_seed;
    const u32 seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution< u32 > distribute(0, 1);

	MPI_Comm nc;
	int n;
	n = distribute(generator);
    std::cout << "### Process " << rank << " has n = " << n << " ###" << std::endl;

    // the creation of a new communicator
	MPI_Comm_split(MPI_COMM_WORLD, n, rank, &nc);
    
	if (n) {
		int a;
        std::uniform_int_distribution< u32 > distribute(1, 1000);
		a = distribute(generator);
        
        int box[2];
        box[0] = rank;
        box[1] = a;

		int nc_size;

        // polling the number of processes in the communication area
		MPI_Comm_size(nc, &nc_size);
		std::vector<int> res(nc_size * 2);

		MPI_Allgather(&box[0], 2, MPI_INT, &res[0], 2, MPI_INT, nc);
        
        std::cout << "!!! Process " << rank << " with n = " << n << ":" << std::endl;
		for (int i = 0; i < res.size(); ++i) {
			if (i % 2 == 0) {
                std::cout << " from " << res[i] << " process: ";
            } else {
                std::cout << res[i] << std::endl;
            }
		}
        std::cout << std::endl;
	}

    MPI_Finalize();

    return 0;
}
