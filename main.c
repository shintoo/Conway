#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "life.h"

int main(int argc, char **argv ) {
	int generations = atoi(argv[1]);
	struct timespec req = {0, 100 * 1000 * 1000};
	struct timespec rem = {0,0};
	FILE *in = fopen(argv[2], "r");

	Field *Conway = NewField();

	Conway->Read(Conway, in);
	
	puts("Generation:\t1");
	Conway->Print(Conway);
	for (int i = 2; i <= generations; i++) {
		Conway->Evolve(Conway);
		printf("\n\nGeneration:\t%d\n", i);
		Conway->Print(Conway);
		nanosleep(&req, &rem);
	}

	return 0;
}

