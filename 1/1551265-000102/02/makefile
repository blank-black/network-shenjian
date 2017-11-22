objects=test1.o test2.o test3.o
test :$(objects)
	cc -o test $(objects)
$(objects):test.h
clean:
	rm test *.o

