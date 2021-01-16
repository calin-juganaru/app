#include "./filter.hpp"

#include <thread>

// ============================================================================

inline static auto partial_sharpen(int threads, int thread_id,
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

exec_time_t thread_sharpen(int THREADS, string input_file, string output_file)
{
    auto start = chrono::high_resolution_clock::now();

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
    auto time = chrono::duration<exec_time_t>(stop - start);

    return time.count();
}

// ============================================================================