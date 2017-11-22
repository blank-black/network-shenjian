test2:libtest1.so test1.o
	cc -o test2 test2.c -L. -ltest1 -Wl,-rpath .
libtest1.so:test1.c
	cc -c -fpic test1.c
	cc -shared -fpic -o libtest1.so test1.o
clean:
	rm test2 *.o *.so
