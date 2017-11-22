test2:libtest1.so test1.o
	g++ -g -Wall -o test2 test2.cpp -L. -ltest1 -Wl,-rpath .
libtest1.so:test1.cpp
	g++ -c -fpic test1.cpp
	g++ -shared -fpic -o libtest1.so test1.o
clean:
	rm test2 *.o *.so
