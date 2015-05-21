CFLAGS = -std=c11 -D_GNU_SOURCE
LDFLAGS = -lm
SRC= src/main.c src/life.c
HDR = src/life.h

all: $(SRC) $(HDR)
	$(CC) -o Conway $(SRC) $(CFLAGS) $(LDFLAGS)
