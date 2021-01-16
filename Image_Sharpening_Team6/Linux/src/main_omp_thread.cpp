#include "./filter.hpp"

#include <thread>
#include <omp.h>

// ============================================================================

void sharpen_local(image_t& image, int omp_threads)
{
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

    image = move(result);
}

// ============================================================================

auto partial_sharpen(int omp_threads, int std_threads, int thread_id,
                     const image_t& original, image_t& result)
{
    auto lines = original.size();
    auto size  = lines / std_threads;
    auto start = thread_id * size;
    auto stop  = start + size;

    if (thread_id == std_threads - 1)
        stop += lines % std_threads;

    if (start > 0) --start;
    if (stop < lines) ++stop;

    auto image = image_t();

    for (auto i = start; i < stop; ++i)
        image.push_back(original[i]);

    sharpen_local(image, omp_threads);

    #pragma omp parallel for shared(result, image) schedule(static)
    for (auto i = start + 1; i < stop - 1; ++i)
        result[i] = move(image[i - start]);

    return nullptr;
}

// ============================================================================

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();

    auto std_threads = stoi(argv[1]);
    auto omp_threads = std_threads / 2;

    std_threads -= omp_threads;
    std_threads = max(std_threads, 1);
    omp_threads = max(omp_threads, 1);

    omp_set_num_threads(omp_threads);

    auto input_file  = string(argv[2]);
    auto output_file = string(argv[3]);

    auto image = parse_input(input_file);

    auto result    = image_t(image.size());
    result.front() = ivec_t(image.front().size());
    result.back()  = ivec_t(image.front().size());

    auto threads = vector<thread>();

    for (auto i = 0; i < std_threads; ++i)
    	threads.emplace_back(partial_sharpen, omp_threads, std_threads,
                             i, cref(image), ref(result));

    for (auto&& thread: threads) thread.join();

    write_image(output_file, result);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double>(stop - start);
    cout << fixed << "Elapsed time: " << time.count() << " sec\n";
}

// ============================================================================