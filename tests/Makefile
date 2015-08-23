LIBS=
LDFLAGS=-lm
CFLAGS=-Wall -lcelegans -L./../ -I./../

SRC=$(wildcard *.c)
OUT=$(SRC:%.c=%)

RM=rm -f
CC=gcc

all: $(OUT)

clean:
	$(RM) $(OUT)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: all
