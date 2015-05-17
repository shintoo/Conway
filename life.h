#include <stdbool.h>
#include <stdio.h>

#define ON 'O'
#define OFF '.'

#define MAXX 100
#define MAXY 100

typedef struct field {
	bool **grid;
	int x;
	int y;

	struct field *(*Clone)(struct field *);
	void (*Read)(struct field *, FILE *);
	void (*Evolve)(struct field *);
	void (*Print)(struct field *);
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

Field * Clone(Field *self);

void SetSize(struct field *, int, int);

void SetGrid(struct field *, char *grid);

void ReadFile(struct field *, FILE *fp);
