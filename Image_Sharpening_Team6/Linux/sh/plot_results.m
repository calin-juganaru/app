close all; clear all; width = 2;

# ==========================================================

serial     = [ 2.6170 * ones(1, 6) ];
mpi        = [ 2.8072 1.8139 1.4894 1.3350 1.2844 1.2262 ];
omp        = [ 2.6200 1.5698 1.2207 1.0442 .96840 .91858 ];
pthreads   = [ 2.6437 1.5793 1.2236 1.0451 .96379 .94209 ];
std_thread = [ 2.6917 1.6265 1.2865 1.1471 1.0154 .99916 ];
omp_thread = [ .90179 .89406 .89629 .90418 .88331 .87815 ];

figure;
plot(serial,     "LineWidth", width); hold on;
plot(mpi,        "LineWidth", width); hold on;
plot(omp,        "LineWidth", width); hold on;
plot(pthreads,   "LineWidth", width); hold on;
plot(std_thread, "LineWidth", width); hold on;
plot(omp_thread, "LineWidth", width); hold on;
title("Nivelul de optimizare: O0");
legend("Serial", "MPI", "OpenMP", "pthread",
       "std::thread", "OMP + std");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");
print -djpg ./tests/exec_time/O0.jpg;

# ==========================================================

serial     = [ .41402 * ones(1, 6) ];
mpi        = [ .63173 .55236 .53889 .52962 .53098 .53555 ];
omp        = [ .43096 .33047 .30451 .28606 .26949 .26856 ];
pthreads   = [ .43026 .32567 .28988 .27461 .26628 .26867 ];
std_thread = [ .46618 .34281 .30150 .28335 .27375 .28613 ];
omp_thread = [ .26406 .26205 .25720 .25497 .25541 .25554 ];

figure;
plot(serial,     "LineWidth", width); hold on;
plot(mpi,        "LineWidth", width); hold on;
plot(omp,        "LineWidth", width); hold on;
plot(pthreads,   "LineWidth", width); hold on;
plot(std_thread, "LineWidth", width); hold on;
plot(omp_thread, "LineWidth", width); hold on;
title("Nivelul de optimizare: O1");
legend("Serial", "MPI", "OpenMP", "pthread",
       "std::thread", "OMP + std");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");
print -djpg ./tests/exec_time/O1.jpg;

# ==========================================================

serial     = [ .38854 * ones(1, 6) ];
mpi        = [ .61846 .54501 .53116 .52539 .52883 .52900 ];
omp        = [ .38855 .30423 .27643 .26284 .25405 .24557 ];
pthreads   = [ .40360 .31181 .28107 .26612 .25887 .26971 ];
std_thread = [ .38289 .29344 .26267 .24875 .24011 .24847 ];
omp_thread = [ .24890 .24918 .24232 .24276 .24309 .24105 ];

figure;
plot(serial,     "LineWidth", width); hold on;
plot(mpi,        "LineWidth", width); hold on;
plot(omp,        "LineWidth", width); hold on;
plot(pthreads,   "LineWidth", width); hold on;
plot(std_thread, "LineWidth", width); hold on;
plot(omp_thread, "LineWidth", width); hold on;
title("Nivelul de optimizare: O2");
legend("Serial", "MPI", "OpenMP", "pthread",
       "std::thread", "OMP + std");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");
print -djpg ./tests/exec_time/O2.jpg;

# ==========================================================

serial     = [ .31648 * ones(1, 6) ];
mpi        = [ .53036 .49458 .48859 .48626 .48672 .49099 ];
omp        = [ .32269 .26755 .24953 .24043 .23512 .22912 ];
pthreads   = [ .33333 .26832 .24765 .23772 .23296 .23778 ];
std_thread = [ .33256 .26854 .24766 .23792 .23343 .23830 ];
omp_thread = [ .23557 .23626 .22992 .22956 .22843 .22848 ];

figure;
plot(serial,     "LineWidth", width); hold on;
plot(mpi,        "LineWidth", width); hold on;
plot(omp,        "LineWidth", width); hold on;
plot(pthreads,   "LineWidth", width); hold on;
plot(std_thread, "LineWidth", width); hold on;
plot(omp_thread, "LineWidth", width); hold on;
title("Nivelul de optimizare: O3");
legend("Serial", "MPI", "OpenMP", "pthread",
       "std::thread", "OMP + std");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");
print -djpg ./tests/exec_time/O3.jpg;

# ==========================================================

serial     = [ .31521 * ones(1, 6) ];
mpi        = [ .52189 .48425 .48041 .48080 .48113 .49561 ];
omp        = [ .30496 .25640 .25441 .24335 .24081 .24879 ];
pthreads   = [ .32432 .26391 .24977 .24097 .23837 .24204 ];
std_thread = [ .32373 .26575 .24840 .24197 .24137 .24178 ];
omp_thread = [ .24890 .24183 .23571 .23144 .23687 .23349 ];

figure;
plot(serial,     "LineWidth", width); hold on;
plot(mpi,        "LineWidth", width); hold on;
plot(omp,        "LineWidth", width); hold on;
plot(pthreads,   "LineWidth", width); hold on;
plot(std_thread, "LineWidth", width); hold on;
plot(omp_thread, "LineWidth", width); hold on;
title("Nivelul de optimizare: Ofast");
legend("Serial", "MPI", "OpenMP", "pthread",
       "std::thread", "OMP + std");
xlabel("Numărul de fire de execuție");
ylabel("Timpul de execuție (secunde)");
print -djpg ./tests/exec_time/Ofast.jpg;

# ==========================================================

