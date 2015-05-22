CFLAGS = -std=c11 -D_GNU_SOURCE
LDFLAGS = -lm
SRC= src/main.c src/conway.c
HDR = src/conway.h

all: $(SRC) $(HDR)
	$(CC) -o Conway $(SRC) $(CFLAGS) $(LDFLAGS)
