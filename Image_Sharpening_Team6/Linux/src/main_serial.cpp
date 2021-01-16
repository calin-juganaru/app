#include "./filter.hpp"

// ============================================================================

int main(int argc, char* argv[])
{
    auto start = chrono::high_resolution_clock::now();

    auto input_file  = string(argv[1]);
    auto output_file = string(argv[2]);

    auto image = parse_input(input_file);
    sharpen(image);
    write_image(output_file, image);

    auto stop = chrono::high_resolution_clock::now();
    auto time = chrono::duration<double>(stop - start);
    cout << fixed << "Elapsed time: " << time.count() << " sec\n";
}

// ============================================================================