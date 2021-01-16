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

    auto v = vector<int>{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10};
    auto length = v.size() / P;
    auto u = vector<int>(length);

    MPI_Scatter(v.data(), length, MPI_INT, u.data(), length, MPI_INT, 0, MPI_COMM_WORLD);

    for (auto i = 1; i < u.size(); ++i)
        u[i] += u[i - 1];

    MPI_Gather(u.data(), length, MPI_INT, v.data(), length, MPI_INT, 0, MPI_COMM_WORLD);

    if (!thread_id)
    {
        auto sum = 0;

        for (auto&& i: v)
            cout << i << ' ';
        cout << endl;

        for (auto i = length - 1; i < v.size(); i += length)
            sum += v[i];

        cout << "Sum = " << sum << endl;
    }

    MPI_Finalize();
}