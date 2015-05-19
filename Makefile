CC=gcc
CFLAGS= -std=gnu11
SRC= src/main.c src/life.c
HDR = src/life.h

all: $(SRC) $(HDR)
	$(CC) -o Conway $(SRC) $(CFLAGS)
