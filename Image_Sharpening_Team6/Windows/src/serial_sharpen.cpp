#include "./filter.hpp"

// ============================================================================

exec_time_t serial_sharpen(string input_file, string output_file)
{
    auto start = chrono::high_resolution_clock::now();

    auto image = parse_input(input_file);
    sharpen(image);
    write_image(output_file, image);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration<exec_time_t>(stop - start);

    return time.count();
}

// ============================================================================