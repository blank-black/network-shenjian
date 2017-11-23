SRCS = $(wildcard *.c)
BUILD = $(patsubst %.c, %, $(SRCS))
CC = cc

.cpp:
	$(CC) -o $@ $<

all: $(BUILD)

clean:
	$(RM) $(BUILD)
