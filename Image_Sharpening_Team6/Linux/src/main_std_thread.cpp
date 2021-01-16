#include "./filter.hpp"

#include <thread>

// ============================================================================

auto partial_sharpen(int threads, int thread_id,
                     const image_t& original, image_t& result)
{
    auto lines = original.size();
    auto size  = lines / threads;
    auto start = thread_id * size;
    auto stop  = start + size;

    if (thread_id == threads - 1)
        stop += lines % threads;

    if (start > 0) --start;
    if (stop < lines) ++stop;

    auto image = image_t();

    for (auto i = start; i < stop; ++i)
        image.push_back(original[i]);

    sharpen(image);

    for (auto i = start + 1; i < stop - 1; ++i)
        result[i] = move(image[i - start]);
}

// ============================================================================

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();

    auto THREADS     =   stoi(argv[1]);
    auto input_file  = string(argv[2]);
    auto output_file = string(argv[3]);

    auto image = parse_input(input_file);

    auto result    = image_t(image.size());
    result.front() = ivec_t(image.front().size());
    result.back()  = ivec_t(image.front().size());

    auto threads = vector<thread>();

    for (auto i = 0; i < THREADS; ++i)
    	threads.emplace_back(partial_sharpen, THREADS, i,
                             cref(image), ref(result));

    for (auto&& thread: threads) thread.join();

    write_image(output_file, result);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double>(stop - start);
    cout << fixed << "Elapsed time: " << time.count() << " sec\n";
}

// ============================================================================