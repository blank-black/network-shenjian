SRCS = $(wildcard *.cpp)
BUILD = $(patsubst %.cpp, %, $(SRCS))
CC = g++

.cpp:
	$(CC) -o $@ $<

all: $(BUILD)

clean:
	$(RM) $(BUILD)
