a.out:
	gcc -c -g ../hashtable.c
	ar -r libxx.a hashtable.o
	g++ -g3 1.cpp -ljemalloc -lxx -L. -std=c++11
