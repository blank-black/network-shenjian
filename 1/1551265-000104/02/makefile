test2:test1.so test1.o
	g++ -o test2 test2.cpp test1.so
test1.so:test1.cpp
	g++ -c -fpic test1.cpp
	g++ -shared -fpic -o test1.so test1.o
#	cp test1.so /usr/lib
clean:
	rm test2 *.o *.so
