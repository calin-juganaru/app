#include "./filter.hpp"

#include <mpi.h>
#include <omp.h>

// ============================================================================

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();

    MPI_Init(nullptr, nullptr);

    auto thread_id = -1, THREADS = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &thread_id);
    MPI_Comm_size(MPI_COMM_WORLD, &THREADS);
    THREADS = max(THREADS, 1);

    auto input_file  = string(argv[1]);
    auto output_file = string(argv[2]);

    if (!thread_id)
    {
        auto image = parse_input(input_file);
        auto N = image.size() - 2u;
        auto M = image.front().size() - 2u;

        MPI_Datatype NEW_TYPE;
        MPI_Type_vector(M + 2u, 3u, 3u, MPI_CHAR, &NEW_TYPE);
        MPI_Type_commit(&NEW_TYPE);

        omp_set_num_threads(THREADS);

        #pragma omp parallel for schedule(static)
        for (auto id = 1; id < THREADS; ++id)
        {
            MPI_Send(&M, 1, MPI_UNSIGNED, id, 0, MPI_COMM_WORLD);
            MPI_Send(&N, 1, MPI_UNSIGNED, id, 1, MPI_COMM_WORLD);

            auto size  = N / THREADS;
            auto start = id * size;
            auto stop  = start + size;

            if (id == THREADS - 1)
                stop += N % THREADS + 1;

            #pragma omp parallel for schedule(static)
            for (auto i = start - 1; i <= stop; ++i)
                MPI_Send(image[i].data(), 1, NEW_TYPE, id, i, MPI_COMM_WORLD);
        }

        auto aux = N / THREADS + (THREADS == 1 ? 2 : 1);
        while (image.size() > aux)
            image.pop_back();
        sharpen(image);
        image.pop_back();

        #pragma omp parallel for schedule(static)
        for (auto id = 1; id < THREADS; ++id)
        {
            auto size = N / THREADS;
            auto start = id * size;
            auto stop = start + size;

            if (id == THREADS- 1)
                stop += N % THREADS+ 1;

            #pragma omp parallel for schedule(static)
            for (auto i = start; i < stop; ++i)
            {
                auto recv = ivec_t(M + 2);
                MPI_Recv(recv.data(), 1, NEW_TYPE, id, i,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                image.push_back(move(recv));
            }
        }

        image.push_back(ivec_t(M + 2));
        write_image(output_file, image);

        auto stop = chrono::high_resolution_clock::now();
        auto time = chrono::duration<double>(stop - start);
        cout << fixed << "Elapsed time: " << time.count() << " sec\n";
    }
    else
    {
        auto N = 0u, M = 0u;
        auto image = image_t();

        MPI_Recv(&M, 1, MPI_UNSIGNED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&N, 1, MPI_UNSIGNED, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Datatype NEW_TYPE;
        MPI_Type_vector(M + 2u, 3u, 3u, MPI_CHAR, &NEW_TYPE);
        MPI_Type_commit(&NEW_TYPE);

        auto size  = N / THREADS;
        auto start = thread_id * size;
        auto stop  = start + size;

        if (thread_id == THREADS- 1)
            stop += N % THREADS+ 1;

        for (auto i = start - 1; i <= stop; ++i)
        {
            auto recv = ivec_t(M + 2);
            MPI_Recv(recv.data(), 1, NEW_TYPE, 0, i,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            image.push_back(move(recv));
        }

        sharpen(image);

        for (auto i = start; i < stop; ++i)
            MPI_Send(image[i - start + 1].data(), 1,
                     NEW_TYPE, 0, i, MPI_COMM_WORLD);

        image.clear();
    }

    MPI_Finalize();
}

// ============================================================================