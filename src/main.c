#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "life.h"
                      
#define COLORSPEED 14 /* The higher this is, the slower the color changes.
                       * Comment it out to remove color.
                       */

#define LOG 20        /* Stops the game when the field reaches a static
                       * state, writes how many generations it took to reach
                       * in a logfile called log.txt. The amount of games
                       * played and logged is specfied by this constant.
                       * Comment out to neithor log nor stop the inital game.
                       */

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
#ifdef LOG
	FILE *logfile = fopen("log.txt", "w");
	int iterations = 0;
#endif
	unsigned long prevliv;
	int samecount = 0;

	Field *Conway = NewField();
#ifdef LOG
while (iterations < LOG) {
#endif
	if ((strncmp(argv[3], "random", 6)) == 0) {
		Conway->Random(Conway, argv[4], atoi(argv[5]));
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
#ifdef LOG
		if (Conway->LiveCount(Conway) == prevliv) {
			samecount++;
			if (samecount == 20) {
				break;
			}
		}
		else {
			samecount = 0;
		}
#endif
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
		prevliv = Conway->LiveCount(Conway);
		nanosleep(&req, &rem);
	}
#ifdef LOG
	fprintf(logfile, "%3d %lu\n", iterations, i - samecount);
	sleep(1);
	iterations++;
}	
	fclose(logfile);
#endif
	return 0;
}

