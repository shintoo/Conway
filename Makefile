CC=gcc
CFLAGS= -std=gnu11
SRC= main.c life.c
HDR = life.h
OBJ= main.o life.o

all: $(SRC) $(HDR)
	$(CC) -o Life $(SRC) $(CFLAGS)
