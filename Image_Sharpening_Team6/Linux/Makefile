FLAGS = -std=c++17
PYTHON = python3
OPT = -Ofast
IMAGE = landscape
THREADS = 5
SRC_DIR = ./src
HEADERS = $(SRC_DIR)/filter.hpp
OBJ_DIR = ./obj
SH_DIR = ./sh
IN_OUT = ./in/$(IMAGE).pnm ./$(IMAGE).pnm
REF_OUT = ./ref/$(IMAGE).pnm ./$(IMAGE).pnm

###############################################################################

build: main_serial main_mpi main_omp main_pthreads main_std_thread main_mpi_omp main_omp_thread $(HEADERS)

main_serial: $(SRC_DIR)/main_serial.cpp
	g++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@

main_mpi: $(SRC_DIR)/main_mpi.cpp
	mpic++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@

main_omp: $(SRC_DIR)/main_omp.cpp
	g++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@ -fopenmp

main_pthreads: $(SRC_DIR)/main_pthreads.cpp
	g++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@ -pthread

main_std_thread: $(SRC_DIR)/main_std_thread.cpp
	g++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@ -pthread

main_mpi_omp: $(SRC_DIR)/main_mpi_omp.cpp
	mpic++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@ -fopenmp

main_omp_thread: $(SRC_DIR)/main_omp_thread.cpp
	g++ $(FLAGS) $(OPT) $^ -o $(OBJ_DIR)/$@ -pthread -fopenmp

clean:
	rm -f $(OBJ_DIR)/* *.pnm

###############################################################################

test_serial: main_serial
	rm -f ./$(IMAGE).pnm
	$(OBJ_DIR)/$^ $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_mpi: main_mpi
	rm -f ./$(IMAGE).pnm
	mpirun -np $(THREADS) $(OBJ_DIR)/$^ $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_omp: main_omp
	rm -f ./$(IMAGE).pnm
	$(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_pthreads: main_pthreads
	rm -f ./$(IMAGE).pnm
	$(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_std_thread: main_std_thread
	rm -f ./$(IMAGE).pnm
	$(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_mpi_omp: main_mpi_omp
	rm -f ./$(IMAGE).pnm
	export OMP_NUM_THREADS=$(THREADS)
	mpirun -np $(THREADS) $(OBJ_DIR)/$^ $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

test_omp_thread: main_omp_thread
	rm -f ./$(IMAGE).pnm
	export OMP_NUM_THREADS=$(THREADS)
	$(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)
	$(PYTHON) $(SH_DIR)/compare.py $(REF_OUT)

###############################################################################

valgrind_serial: main_serial
	rm -f ./$(IMAGE).pnm
	valgrind $(OBJ_DIR)/$^ $(IN_OUT)

valgrind_mpi: main_mpi
	rm -f ./$(IMAGE).pnm
	valgrind mpirun -np $(THREADS) $(OBJ_DIR)/$^ $(IN_OUT)

valgrind_omp: main_omp
	rm -f ./$(IMAGE).pnm
	export OMP_NUM_THREADS=$(THREADS)
	valgrind $(OBJ_DIR)/$^ $(IN_OUT)

valgrind_pthreads: main_pthreads
	rm -f ./$(IMAGE).pnm
	valgrind $(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)

valgrind_std_thread: main_std_thread
	rm -f ./$(IMAGE).pnm
	valgrind $(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)

valgrind_omp_thread: main_omp_thread
	rm -f ./$(IMAGE).pnm
	valgrind $(OBJ_DIR)/$^ $(THREADS) $(IN_OUT)

###############################################################################