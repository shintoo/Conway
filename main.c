#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>

#include "life.h"

int main(int argc, char **argv ) {
	if (argc < 4) {
		printf("usage: %s <generations> <speed> <text file>\n",
		       argv[0]);
		exit(EXIT_FAILURE);
	}
	
	int generations = atoi(argv[1]);
	
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
	int randx, randy;
	FILE *in;

	if (strncmp(argv[3], "random", 6) == 0) {

		random = true;
	}
	else if ((in = fopen(argv[3], "r")) == NULL) {
		fprintf(stderr, "%s not found.\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	Field *Conway = NewField();
	switch (random) {
//		case true: Conway->Random(Conway); break;
		case false: Conway->Read(Conway, in); break;
	}

	puts("Generation:\t1");
	Conway->Print(Conway);
	for (int i = 2; i <= generations || (generations == 0 && i < INT_MAX); i++) {
		Conway->Evolve(Conway);
		printf("\n\nGeneration:\t%d\n", i);
		Conway->Print(Conway);
		nanosleep(&req, &rem);
	}

	return 0;
}

