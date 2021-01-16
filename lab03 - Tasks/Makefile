build: sections.c
	gcc sections.c -o sections -fopenmp

ex: ex_sections.c
	gcc ex_sections.c -o ex_sections -fopenmp -lm

run: clean build
	export OMP_NUM_THREADS=4
	./sections

clean:
	rm -f sections.c.* sections ex_sections