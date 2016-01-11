a.out:
	gcc -c -g ../queue.c --static -ljemalloc
	ar -r libxx.a queue.o
	g++ -g3 1.cpp -lxx -L. -std=c++11
