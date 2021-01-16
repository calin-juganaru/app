build:
	gcc matrix_mult.c -o matrix_mult -fopenmp -fdump-tree-all

run: clean build
	export OMP_NUM_THREADS=4
	./matrix_mult

clean:
	rm -f matrix_mult.c.* matrix_mult