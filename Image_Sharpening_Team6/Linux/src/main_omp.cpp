#include "./filter.hpp"

#include <omp.h>

// ============================================================================

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();

    auto THREADS     =   stoi(argv[1]);
    auto input_file  = string(argv[2]);
    auto output_file = string(argv[3]);

    omp_set_num_threads(THREADS);

    auto image = parse_input(input_file);

    auto N = image.size();
    auto M = image.front().size();
    auto result = image_t(N, ivec_t(M));

    #pragma omp parallel for schedule(static)
    for (auto i = 1u; i < N - 1u; ++i)
        #pragma omp parallel for schedule(static)
        for (auto j = 1u; j < M - 1u; ++j)
        {
            auto aux = ftrip_t(0.0f);

            for (auto x: {-1, 0, 1})
                for (auto y: {-1, 0, 1})
                    aux += image[i + x][j + y] * filter[1 - x][1 - y];

            if (aux.first  < 0) aux.first  = 0;
            if (aux.second < 0) aux.second = 0;
            if (aux.third  < 0) aux.third  = 0;

            if (aux.first  > 255) aux.first  = 255;
            if (aux.second > 255) aux.second = 255;
            if (aux.third  > 255) aux.third  = 255;

            result[i][j] = aux;
        }

    write_image(output_file, result);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double>(stop - start);
    cout << fixed << "Elapsed time: " << time.count() << " sec\n";
}

// ============================================================================