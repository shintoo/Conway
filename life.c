#include "life.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static int GetNeighbors(Field *gol, int x, int y);

Field * NewField(void) {
	Field *new = malloc(sizeof(Field));
	
	new->x = 0;
	new->y = 0;

	new->Read = ReadFile;
	new->Print = Print;
	new->Evolve = Evolve;
	new->SetGrid = SetGrid;
	new->SetSize = SetSize;
	new->Clone = Clone;

	return new;
}

void ReadFile(Field *self, FILE *fp) {
	char ch;
	char grid[MAXX * MAXY + MAXY];
	int xsize = 0;
	int ysize = 0;
	int i = 0;

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n') {
			ysize++;
		}
		else {
			xsize++;
		}
		grid[i++] = ch;
	}
	xsize = xsize / ysize;
	grid[i] = '\0';

	self->SetSize(self, xsize, ysize);
	self->SetGrid(self, grid);
}

Field * Clone(Field *self) {
	Field *clone = NewField();
	
	clone->SetSize(clone, self->x, self->y);
	
	for (int i = 0; i < clone->y; i++) {
		for (int j = 0; j < clone->x; j++) {
			clone->grid[i][j] = self->grid[i][j];
		}
	}

	return clone;
}

bool WillLive(Field *self, int x, int y) {
	if (self->grid[y][x] == false) {
		if (GetNeighbors(self, x, y) == 3) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		switch(GetNeighbors(self, x, y)) {
			case 0 ... 1: return false;
			case 2 ... 3: return true;
			case 4 ... 8: return false;
		}
	}
	return false;
}

void Evolve(Field *self) {
	Field *temp = self->Clone(self);

	for (int i = 1; i < self->y - 1; i++) {
		for (int j = 1; j < self->x - 1; j++) {
			self->grid[i][j] = WillLive(temp, j, i);
		}
	}
}

void Print(Field *self) {	
	for (int i = 0; i < self->y; i++) {
		for (int j = 0; j < self->x; j++) {
			if (self->grid[i][j] == true) {
				printf("█");
			}
			else if (self->grid[i][j] == false) {
				printf("░");
			}
		}
		putchar('\n');
	}
}

void SetSize(Field *self, int x, int y) {
	self->grid = malloc(y * sizeof(bool *));
	for (int i = 0; i < y; i++) {
		self->grid[i] = malloc(x * sizeof(bool));
	}
	self->x = x;
	self->y = y;
}

void SetGrid(Field *self, char *rows) {
	int i = 0;
	for (int y = 0; y < self->y; y++) {
		for (int x = 0; x < self->x; x++) {
			switch(rows[i++]) {
				case ON: self->grid[y][x] = true; break;
				case OFF: self->grid[y][x] = false; break;
				case '\n': x--; break;
				case '\0': return;
			}
		}
	}
}

void ClearScreen(void) {
	const char * CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
	write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}
static int GetNeighbors(Field *self, int x, int y) {
		int ct = 0;
		
		if (self->grid[y][x - 1] == true) {             /* adjacents */
			ct++; 
		} 
		if (self->grid[y][x + 1] == true) {
			ct++;                                       
		}
		if (self->grid[y - 1][x] == true) {
			ct++;
		}
		
		if (self->grid[y + 1][x] == true) {
			ct++;
		}

		if (self->grid[y - 1][x - 1] == true) {         /* diagonal */
			ct++;
		}
		if (self->grid[y - 1][x + 1] == true) {
			ct++;
		}
		if (self->grid[y + 1][x + 1] == true) {
			ct++;
		}
		if (self->grid[y + 1][x - 1] == true) {
			ct++;
		}

		return ct;
}
