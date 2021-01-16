#include <mpi.h>

#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    int thread_id, P;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &P);

    auto v = vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto length = v.size() / (P - 1);

    if (thread_id)
    {
        auto aux = 0;

        auto start = (thread_id - 1) * length;
	    auto stop = start + length;

        if (thread_id == P - 1) stop += v.size() % (P - 1);

        for (auto i = start; i < stop; ++i)
            aux += v[i];

        //cout << thread_id << ' ' << start << ' ' << stop << ' ' << aux << endl;

        MPI_Send(&aux, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
    else
    {
        auto sum = 0;
        for (auto i = 1; i < P; ++i)
        {
            auto aux = 0;
            MPI_Recv(&aux, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum += aux;
        }

        cout << "Sum = " << sum << endl;
    }

    MPI_Finalize();
}