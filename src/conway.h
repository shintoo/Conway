#ifndef _LIFE_H_
#define _LIFE_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#define ON 'O'
#define OFF '.'

#define MAXX 300
#define MAXY 100

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

typedef struct field {
	bool **grid;
	int sizex;
	int sizey;
	unsigned long alive;

	struct field *(*Clone)(struct field *);
	
	void (*Read)(struct field *, FILE *);
	void (*Random)(struct field *, char *, int);

	void (*Evolve)(struct field *);
	void (*Print)(struct field *);
	
	unsigned long (*LiveCount)(struct field *);
	unsigned long (*Total)(struct field *);

	void (*SetSize)(struct field *, int, int);
	void (*SetGrid)(struct field *, char *grid);
} Field;

void ClearScreen(void);

/* initialize a field for Conway's GoL */
Field * NewField(void);

/* determines if the cell should be alive in the next generation */
bool WillLive(Field *self, int x, int y);

/* print the grid */
void Print(Field *self);

/* refresh grid to next generation */
void Evolve(Field *self);

unsigned long LiveCount(Field *self);

unsigned long Total(Field *self);

Field * Clone(Field *self);

void DeleteField(Field *self);

void SetSize(struct field *, int, int);

void SetGrid(struct field *, char *grid);

void ReadFile(struct field *, FILE *fp);

bool getcell(struct field *, int, int);

void Random(struct field *, char *dim, int density);
#endif
