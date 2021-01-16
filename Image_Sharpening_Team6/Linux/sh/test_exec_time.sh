#!/bin/bash

IMAGE='landscape'
MAX_THREADS=6
REPS=100
OBJ_DIR='./obj'

printf "close all; clear all; width = 2;\n"
printf "\n# ==========================================================\n\n"

###############################################################################

function test_time
{
    printf "$1 = [ "
    program="$2"

    for THREADS in $(seq 1 $MAX_THREADS)
    do
        mean=0

        if [[ "$program" == *serial ]]; then THREADS=''; fi

        for rep in $(seq 1 $REPS)
        do
            aux="$program $THREADS"

            if [[ "$program" == *_mpi* ]]
            then
                aux="mpirun -np $THREADS $program"
            fi

            aux=$($aux ./in/$IMAGE.pnm ./$IMAGE.pnm/)
            aux=$(echo "$aux" | cut -d ' ' -f 3)
            aux=$(bc -l <<< "${aux}")
            mean=$(bc -l <<<"${mean}+${aux}")
        done

        mean=$(bc -l <<<"${mean}/${REPS}")
        mean=$(echo $mean | head -c6)
        printf "$mean "

        if [[ "$program" == *serial ]]
        then
            printf "* ones(1, 6) "
            break
        fi
    done

    printf "];\n"
}

###############################################################################

for OPTS in "O0" "O1" "O2" "O3" "Ofast"
do
    make clean &> /dev/null
    make OPT="-$OPTS" &> /dev/null

    test_time "serial    " "$OBJ_DIR/main_serial"
    test_time "mpi       " "$OBJ_DIR/main_mpi"
    test_time "omp       " "$OBJ_DIR/main_omp"
    test_time "pthreads  " "$OBJ_DIR/main_pthreads"
    test_time "std_thread" "$OBJ_DIR/main_std_thread"
    test_time "omp_thread" "$OBJ_DIR/main_omp_thread"

    printf "\nfigure;\n"
    printf "plot(serial,     \"LineWidth\", width); hold on;\n"
    printf "plot(mpi,        \"LineWidth\", width); hold on;\n"
    printf "plot(omp,        \"LineWidth\", width); hold on;\n"
    printf "plot(pthreads,   \"LineWidth\", width); hold on;\n"
    printf "plot(std_thread, \"LineWidth\", width); hold on;\n"
    printf "plot(omp_thread, \"LineWidth\", width); hold on;\n"

    printf "title(\"Nivelul de optimizare: $OPTS\");\n"
    printf "legend(\"Serial\", \"MPI\", \"OpenMP\", \"pthread\",\n"
    printf "       \"std::thread\", \"OMP + std\");\n"
    printf "xlabel(\"Numărul de fire de execuție\");\n"
    printf "ylabel(\"Timpul de execuție (secunde)\");\n"
    printf "print -djpg ./tests/exec_time/$OPTS.jpg;\n"

    printf "\n# ==========================================================\n\n"
done

###############################################################################