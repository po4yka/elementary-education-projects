#include "mpi.h"
#include <vector>

#define SEND_ARR_SIZE 5

int main(int ac,char **av)
{
    int size, rank;

    MPI_Init(&ac, &av);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size == 0)
    {
        std::cout << "Mo way to execute with 0 processes" << std::endl;
        return 0;
    }

    std::vector<int> n(SEND_ARR_SIZE);
    n[0] = rank;
    for (int i = 1; i < SEND_ARR_SIZE; ++i)
    {
        n[i] = i;
    }

    int t = SEND_ARR_SIZE * size;
    std::vector<int> res(t);

    MPI_Allgather(&n[0], SEND_ARR_SIZE, MPI_INT, &res[0], SEND_ARR_SIZE, MPI_INT, MPI_COMM_WORLD);

    std::cout << "------------" << std::endl;
    std::cout << "|Process: " << rank << "|" << std::endl;
    std::cout << "------------";
    for (int i = 0; i < res.size(); ++i)
    {
        if (i % SEND_ARR_SIZE == 0) {
            std::cout << std::endl << ">from: " << res[i] << " process: ";
        } else {
            std::cout << res[i] << " ";
        }
    }
    std::cout << std::endl;

    MPI_Finalize();

    return 0;
}
