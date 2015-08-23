LIBS=
LDFLAGS=-lm
CFLAGS=-Wall -fPIC

SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=%.o)

RM=rm -f
CC=gcc
MAKE=make

all: main lib tests

clean:
	$(RM) $(OBJ) main libcelegans.so
	$(MAKE) -C ./tests clean

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o main

lib: $(OBJ)
	$(CC) $(LDFLAGS) $(filter-out main.o,$(OBJ)) -shared -o libcelegans.so

tests: lib
	$(MAKE) -C ./tests

.PHONY: all
