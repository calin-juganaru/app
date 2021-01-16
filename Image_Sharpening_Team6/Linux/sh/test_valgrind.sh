#!/bin/bash

IMAGE='landscape'
THREADS=6
OPTS='-O0'
OBJ_DIR='./obj'

###############################################################################

make clean &> /dev/null
make OPT=$OPTS &> /dev/null

echo ""
echo "Optimise level = $OPTS"

echo ""
echo "  @main_serial:"
valgrind $OBJ_DIR/main_serial ./in/$IMAGE.pnm ./$IMAGE.pnm

echo ""
echo "  @main_mpi:"
valgrind mpirun -np $THREADS $OBJ_DIR/main_mpi ./in/$IMAGE.pnm ./$IMAGE.pnm
echo ""

echo "  @main_omp:"
export OMP_NUM_THREADS=$THREADS
valgrind $OBJ_DIR/main_omp ./in/$IMAGE.pnm ./$IMAGE.pnm
echo ""

echo "  @main_pthreads:"
valgrind $OBJ_DIR/main_pthreads $THREADS ./in/$IMAGE.pnm ./$IMAGE.pnm
echo ""

echo "  @main_std_thread:"
valgrind $OBJ_DIR/main_std_thread $THREADS ./in/$IMAGE.pnm ./$IMAGE.pnm
echo ""

echo "  @main_pthreads_omp:"
export OMP_NUM_THREADS=$THREADS
valgrind $OBJ_DIR/main_pthreads_omp $THREADS ./in/$IMAGE.pnm ./$IMAGE.pnm
echo ""

###############################################################################