#include <stdbool.h>

typedef struct field {
	bool **grid;
	int x;
	int y;

	struct field *(*Clone)(struct field *);
	void (*Evolve)(struct field *);
	void (*Print)(struct field *);
	void (*SetSize)(struct field *, int, int);
	void (*SetGrid)(struct field *, char *grid);
} Field;


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

void Show(Field *);
