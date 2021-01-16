#include <mpi.h>

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int process_id, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    cout << "Process " << process_id << " says hello!\n";

    MPI_Finalize();
}