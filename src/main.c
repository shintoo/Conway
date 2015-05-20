#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include "life.h"
                      
#define COLORSPEED 14  /* The higher this is, the slower the color changes.
                        * Comment it out to remove color.
                        */

#define LOG 200         /* Stops the game when the field reaches a static
                        * state, writes how many generations it took to reach
                        * in a logfile called log.txt. The amount of games
                        * played and logged is specfied by this constant.
                        * Comment out to neithor log nor stop the inital game.
                        */

#define LOGACCURACY 40 /* The higher this is, the more generations are tested
                        * to determine a static field. Slower, but more
                        * accurate. There is always the possibility of
                        * a glider spawning with nothing but one 2x2 block
                        * on the field. The larger your field, the higher
                        * LOGACCURACY recommended.
                        */

#define LOGFEATHER 4   /* Blinkers may cause a game to never stop. This
                        * constant allows for changes in life within a
                        * certain range while still considering the
                        * field to be static.
                        */

//#define BACKGROUND     /* When active, this prevents the games from being
/*                        * printed, so the program only runs and logs the
                        * results. Much faster than printing them.
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
	unsigned int iterations = 0;

#ifdef LOG
	FILE *logfile = fopen("log.txt", "w");
	fprintf(logfile, " Game\t Gen\n");
	unsigned long prevliv;
	unsigned long peak = 0;
	unsigned int samecount = 0;
#endif

	Field *Conway = NewField();

#ifdef LOG
while (iterations < LOG) {
#endif

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

#ifndef BACKGROUND
#ifdef COLORSPEED
	puts(RED);
#endif
	puts("Generation:\t1");
	Conway->Print(Conway);
#endif

	unsigned long i = 2;
	for (; i <= generations || (generations == 0 && i < ULONG_MAX) ; i++) {
		Conway->Evolve(Conway);

#ifdef LOG
		if (abs(Conway->LiveCount(Conway) - prevliv) < LOGFEATHER + 1) {
			samecount++;
			if (samecount == LOGACCURACY) {
				break;
			}
		}
		else {
			samecount = 0;
		}
#endif

#ifndef BACKGROUND
#ifdef COLORSPEED
		if (i % COLORSPEED == 0) {
			puts(colors[i % 5]);
		}
#endif
#endif

#ifndef BACKGROUND
#ifdef LOG
		printf("\n\nGame: 24\n");// iterations);
#endif
		printf("Generation:\t%lu\nLive: %lu\t%.3g%%\n",
		       i, Conway->LiveCount(Conway),
		       ((float)Conway->LiveCount(Conway)) /
		       ((float) Conway->Total(Conway)) * 100);
		Conway->Print(Conway);
#endif

#ifdef LOG
		if (Conway->LiveCount(Conway) > peak) {
			peak = Conway->LiveCount(Conway);
		}
		prevliv = Conway->LiveCount(Conway);
#endif

#ifndef BACKGROUND
		nanosleep(&req, &rem);
#endif

	}

#ifdef LOG
	fprintf(logfile, "%4d\t%4lu\n", iterations, i - samecount);
	fclose(logfile);
	fopen("log.txt", "a");
	sleep(1);
	iterations++;
#ifdef BACKGROUND
	printf("%u/%d\n", iterations, LOG);
#endif

}
	fclose(logfile);
#endif
	return 0;
}

