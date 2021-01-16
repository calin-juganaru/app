#include "./filter.hpp"

// ============================================================================

int main()
{
    auto max_threads = 6, REPS = 100;
    auto input_file  = "./in/landscape.pnm"s;
    auto output_file = "./landscape.pnm"s;

    auto optimisation = "O2"s;
    auto image_file = optimisation + ".jpg";
    auto output = ofstream();
    output.open("./tests/plot_results.m", ios_base::app);

    output << "close all; clear all; width = 2;\n\n";

    // ------------------------------------------------------------------------

    {
        output << "serial_sharpen = [ ";
        auto result = exec_time_t();
        for (auto i = 0; i < REPS; ++i, cout << '#')
            result += serial_sharpen(input_file, output_file);
        output << result / static_cast<exec_time_t>(REPS)
            << " * ones(1, " << max_threads << ")];\n";
    }
    cout << endl << endl;

    // ------------------------------------------------------------------------

    output << "thread_sharpen = [ ";
    for (auto threads = 1; threads <= max_threads; ++threads)
    {
        auto result = exec_time_t();
        for (auto i = 0; i < REPS; ++i, cout << '#')
            result += thread_sharpen(threads, input_file, output_file);
        output << result / static_cast<exec_time_t>(REPS) << ' ';
        cout << endl;
    }
    output << "];\n"; cout << endl << endl;

    // ------------------------------------------------------------------------

    output << "openmp_sharpen = [ ";
    for (auto threads = 1; threads <= max_threads; ++threads)
    {
        auto result = exec_time_t();
        for (auto i = 0; i < REPS; ++i, cout << '#')
            result += openmp_sharpen(threads, input_file, output_file);
        output << result / static_cast<exec_time_t>(REPS) << ' ';
        cout << endl;
    }
    output << "];\n"; cout << endl << endl;

    // ------------------------------------------------------------------------

    output << "hybrid_sharpen = [ ";
    for (auto threads = 1; threads <= max_threads; ++threads)
    {
        auto result = exec_time_t();
        for (auto i = 0; i < REPS; ++i, cout << '#')
            result += hybrid_sharpen(threads, input_file, output_file);
        output << result / static_cast<exec_time_t>(REPS) << ' ';
        cout << endl;
    }
    output << "];\n"; cout << endl << endl;

    // ------------------------------------------------------------------------

    output << "\nfigure;\n";
    output << "plot(serial_sharpen, \"LineWidth\", width); hold on;\n";
    output << "plot(thread_sharpen, \"LineWidth\", width); hold on;\n";
    output << "plot(openmp_sharpen, \"LineWidth\", width); hold on;\n";
    output << "plot(hybrid_sharpen, \"LineWidth\", width); hold on;\n\n";

    output << "title(\"Nivelul de optimizare: " << optimisation << "\");\n";
    output << "legend(\"Serial\", \"std::thread\", \"OpenMP\", \"Hybrid\");\n";
    output << "xlabel(\"Numărul de fire de execuție\");\n";
    output << "ylabel(\"Timpul de execuție (secunde)\");\n\n";

    output << "print -djpg " << image_file << ";\n\n";

    output << "% ======================================="
           << "======================================\n\n";
}

// ============================================================================