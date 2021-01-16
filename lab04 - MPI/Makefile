unu:
	mpic++ -std=c++20 hello.cpp -o hello
	mpirun -np 5 ./hello

doi:
	mpic++ -std=c++20 sum.cpp -o sum
	mpirun -np 5 ./sum

trei:
	mpic++ -std=c++20 sum2.cpp -o sum2
	mpirun -np 5 ./sum2

patru:
	mpic++ -std=c++20 sum3.cpp -o sum3
	mpirun -np 3 ./sum3

clean:
	rm -f hello sum sum2 sum3