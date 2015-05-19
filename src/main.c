#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#define COLORS

#include "life.h"

#define COLORSPEED 12

int main(int argc, char **argv ) {
	if (argc < 4) {
		printf("usage: %s <generations> <speed> <<text file> |"
		       " <random X,Y>>\n",
		       argv[0]);
		exit(EXIT_FAILURE);
	}
	char *colors[5] = {RED, YELLOW, GREEN, MAGENTA, CYAN};
	long generations = strtoul(argv[1], NULL, 0);
	
	int speed = atoi(argv[2]);
	if (speed > 10) {
		speed = 10;
	}
	if (speed < 1) {
		speed = 1;
	}
	speed = 11 - speed;

	struct timespec req = {0, speed * 50 * 1000 * 1000};
	struct timespec rem = {0,0};

	bool random;
	FILE *in;

	Field *Conway = NewField();
	if ((strncmp(argv[3], "random", 6)) == 0) {
		Conway->Random(Conway, argv[4], 10);
	}
	else if ((in = fopen(argv[3], "r")) != NULL) {
		Conway->Read(Conway, in);
	}
	else {
		fprintf(stderr, "%s not found.\n", argv[3]);
		exit(EXIT_FAILURE);
	}
#ifdef COLORSPEED
	puts(RED);
#endif
	puts("Generation:\t1");
	Conway->Print(Conway);
	unsigned long i = 2;
	for (; i <= generations || (generations == 0 && i < ULONG_MAX) ; i++) {
		Conway->Evolve(Conway);
#ifdef COLORSPEED
		if (i % COLORSPEED == 0) {
			puts(colors[i % 5]);
		}
#endif
		printf("\n\nGeneration:\t%lu\nLive: %lu\t%.3g%%\n",
		       i, Conway->LiveCount(Conway),
		       ((float)Conway->LiveCount(Conway)) /
		       ((float) Conway->Total(Conway)) * 100);
		Conway->Print(Conway);
		nanosleep(&req, &rem);
	}
	return 0;
}

