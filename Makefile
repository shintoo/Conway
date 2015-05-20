CC=gcc
CFLAGS= -std=gnu11
SRC= src/main.c src/life.c
HDR = src/life.h
LIB= -lm

all: $(SRC) $(HDR)
	$(CC) -o Conway $(SRC) $(CFLAGS) $(LIB)
