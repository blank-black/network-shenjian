test2:test1.so test1.o
	cc -o test2 test2.c test1.so
test1.so:test1.c
	cc -c -fpic test1.c
	cc -shared -fpic -o test1.so test1.o
clean:
	rm test2 *.o *.so
