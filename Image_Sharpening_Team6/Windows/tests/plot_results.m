close all; clear all; width = 2;

serial_sharpen = [ 9.90794 * ones(1, 6)];
thread_sharpen = [ 9.71863 7.13826 6.33761 5.90115 5.6411 5.55169 ];
openmp_sharpen = [ 9.82964 9.82792 9.87546 9.98333 10.0461 9.98728 ];
hybrid_sharpen = [ 9.73672 9.74232 7.21969 7.20658 6.3408 6.37169 ];

figure;
plot(serial_sharpen, "LineWidth", width); hold on;
plot(thread_sharpen, "LineWidth", width); hold on;
plot(openmp_sharpen, "LineWidth", width); hold on;
plot(hybrid_sharpen, "LineWidth", width); hold on;

title("Nivelul de optimizare: Od");
legend("Serial", "std::thread", "OpenMP", "Hybrid");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");

print -djpg Od.jpg;

% =============================================================================

serial_sharpen = [ 4.38312 * ones(1, 6)];
thread_sharpen = [ 4.21805 3.51958 3.33537 3.25902 3.13496 3.11577 ];
openmp_sharpen = [ 4.19078 4.18697 4.16224 4.17277 4.24161 4.33693 ];
hybrid_sharpen = [ 4.268 4.25571 3.53751 3.57492 3.29694 3.32187 ];

figure;
plot(serial_sharpen, "LineWidth", width); hold on;
plot(thread_sharpen, "LineWidth", width); hold on;
plot(openmp_sharpen, "LineWidth", width); hold on;
plot(hybrid_sharpen, "LineWidth", width); hold on;

title("Nivelul de optimizare: O1");
legend("Serial", "std::thread", "OpenMP", "Hybrid");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");

print -djpg O1.jpg;

% =============================================================================

serial_sharpen = [ 2.76911 * ones(1, 6)];
thread_sharpen = [ 2.5432 2.19305 2.11723 2.11189 2.03869 2.05215 ];
openmp_sharpen = [ 2.52616 2.52913 2.53378 2.49692 2.58797 2.54631 ];
hybrid_sharpen = [ 2.41284 2.41138 2.22546 2.18183 2.09783 2.11914 ];

figure;
plot(serial_sharpen, "LineWidth", width); hold on;
plot(thread_sharpen, "LineWidth", width); hold on;
plot(openmp_sharpen, "LineWidth", width); hold on;
plot(hybrid_sharpen, "LineWidth", width); hold on;

title("Nivelul de optimizare: O2");
legend("Serial", "std::thread", "OpenMP", "Hybrid");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");

print -djpg O2.jpg;

% =============================================================================

serial_sharpen = [ 3.06583 * ones(1, 6)];
thread_sharpen = [ 2.82113 2.15581 2.06897 2.01094 2.0113 1.9811 ];
openmp_sharpen = [ 2.50117 2.49711 2.49531 2.47442 2.51044 2.48905 ];
hybrid_sharpen = [ 2.35127 2.43543 2.1599 2.20588 2.17998 2.1368 ];

figure;
plot(serial_sharpen, "LineWidth", width); hold on;
plot(thread_sharpen, "LineWidth", width); hold on;
plot(openmp_sharpen, "LineWidth", width); hold on;
plot(hybrid_sharpen, "LineWidth", width); hold on;

title("Nivelul de optimizare: Ox");
legend("Serial", "std::thread", "OpenMP", "Hybrid");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");

print -djpg Ox.jpg;

% =============================================================================