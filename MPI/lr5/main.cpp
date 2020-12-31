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

    // random generation
    std::random_device os_seed;
    const u32 seed = os_seed();
    engine generator(seed);
    std::uniform_int_distribution< u32 > distribute(1, 10);

    MPI_Comm nc, nc_sub;
	int res[4];
	int z = distribute(generator);
    std::cout << "z = " << z << std::endl;
	int dims[] = { 2, 2, size / 4 },
		periods[] = { 0, 0, 0 },
		remain_dims[] = { 1, 1, 0 };
	
    MPI_Cart_create(MPI_COMM_WORLD, 3, dims, periods, 0, &nc);
	MPI_Cart_sub(nc, remain_dims, &nc_sub);
	MPI_Gather(&z, 1, MPI_INT, res, 1, MPI_INT, 0, nc_sub);

    int sum = 0;
    for (auto &i : res) {
        sum += i;
    }
    std::cout << "SUM: " << sum << std::endl;

    MPI_Finalize();

    return 0;
}
