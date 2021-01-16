FLAGS = -std=c++11 -pthread

build: boss-worker.o queue.o
	g++ $(FLAGS) $^ -o test

queue.o: queue.cpp queue.h
	g++ $(FLAGS) -c queue.cpp

boss-worker.o: boss-worker.cpp
	g++ $(FLAGS) -c boss-worker.cpp

clean:
	rm -f *.o test