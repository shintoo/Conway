#include "life.h"
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>

uint64_t s[2];
uint64_t xorshift(void);

int GetNeighbors(Field *gol, int x, int y);

Field * NewField(void) {
	Field *new = malloc(sizeof(Field));
	
	new->sizex = 0;
	new->sizey = 0;
	
	new->alive = 0;

	new->Read = ReadFile;
	new->Random = Random;
	new->Print = Print;
	new->Evolve = Evolve;
	new->LiveCount = LiveCount;
	new->Total = Total;
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
		if (ch == ON) {
			self->alive++;
		}
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

void Random(Field *self, char *dim, int density) {
	char str_randx[5], str_randy[5];
	int randx, randy;
	char ch;
	time_t t;
	int g = 0; /* grid char counter */
	int i;

	srand((unsigned) time(&t));
	s[0] = rand();
	s[1] = rand();

	for (i = 0; isdigit(dim[i])&& i < 5; i++) {
		str_randx[i] = dim[i];
	}
	str_randx[i] = '\0';
	dim = &dim[++i];
	for (i = 0; dim[i] && i < 5; i++) {
		str_randy[i] = dim[i];
	}
	str_randy[i] = '\0';

	randx = atoi(str_randx);
	randy = atoi(str_randy);

	self->SetSize(self, randx, randy);
	char grid[randx * randy + randy];
	if (density > 10) {
		density = 10;
	}
	if (density < 1) {
		density = 1;
	}
	density = 12 - density;
	for (int y = 0; y < randy; y++) {
		for (int x = 1; x < randx; x++) {
			int chance = xorshift();
			switch (chance % density) {
				case 0: grid[g++] = ON; self->alive++; break;
				case 1: grid[g++] = OFF; break;
			}
		}
		grid[g++] = '\n';
	}
	grid[g] = '\0';
	printf("xsize: %d ysize: %d", self->sizex, self->sizey);
	self->SetGrid(self, grid);
}

Field * Clone(Field *self) {
	Field *clone = NewField();
	clone->alive = self->alive;

	clone->SetSize(clone, self->sizex, self->sizey);
	
	for (int i = 0; i < clone->sizey; i++) {
		for (int j = 0; j < clone->sizex; j++) {
			clone->grid[i][j] = self->grid[i][j];
		}
	}

	return clone;
}

bool WillLive(Field *self, int x, int y) {
	int neighbors = GetNeighbors(self, x, y);

	if (neighbors == 0) {
		return false;
	}
	if (self->grid[y][x] == false) {
		if (neighbors == 3) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		switch(neighbors) {
			case 0 ... 1: return false;
			case 2 ... 3: return true;
			case 4 ... 8: return false;
		}
	}
	return false;
}

void Evolve(Field *self) {
	Field *temp = self->Clone(self);
	self->alive = 0;

	for (int i = 1; i < self->sizey - 1; i++) {
		for (int j = 1; j < self->sizex - 1; j++) {
			self->grid[i][j] = WillLive(temp, j, i);
			if (self->grid[i][j] == true) {
				self->alive++;
			}
		}
	}
}

void Print(Field *self, char *color) {	
	for (int i = 0; i < self->sizey; i++) {
		for (int j = 0; j < self->sizex; j++) {
			if (self->grid[i][j] == true) {
				printf("%s█", color);
			}
			else if (self->grid[i][j] == false) {
				printf("%s░", color);
			}
		}
		putchar('\n');
	}
}

unsigned long LiveCount(Field *self) {
	return self->alive;
}

unsigned long Total(Field *self) {
	return (unsigned long) self->sizex * self->sizey;
}

void SetSize(Field *self, int x, int y) {
	self->grid = malloc(y * sizeof(bool *));
	for (int i = 0; i < y; i++) {
		self->grid[i] = malloc(x * sizeof(bool));
	}
	self->sizex = x;
	self->sizey = y;
}

void SetGrid(Field *self, char *rows) {
	int i = 0;
	for (int y = 0; y < self->sizey; y++) {
		for (int x = 0; x < self->sizex; x++) {
			switch(rows[i++]) {
				case ON: self->grid[y][x] = true; break;
				case OFF: self->grid[y][x] = false; break;
				case '\n': x--; break;
				case '\0': return;
			}
		}
	}
}

int GetNeighbors(Field *self, int x, int y) {
		int ct = 0;

		if (self->grid[y][x - 1] == true) {
			ct++; 
		} 
		if (self->grid[y][x + 1] == true) {
			ct++;                                       
		}
		if (self->grid[y + 1][x] == true) {
			ct++;
		}
		
		if (self->grid[y - 1][x] == true) {
			ct++;
		}

		if (self->grid[y + 1][x + 1] == true) {
			ct++;
		}
		if (self->grid[y + 1][x - 1] == true) {
			ct++;
		}
		if (self->grid[y - 1][x - 1] == true) {
			ct++;
		}
		if (self->grid[y - 1][x + 1] == true) {
			ct++;
		}

		return ct;
}

bool getcell(Field *self, int x, int y) {
	x = (x % self->sizex) + self->sizex*(x % self->sizex < 0);
	y = (y % self->sizey) + self->sizey*(y % self->sizey < 0);
	if (x < 0) {
		x = x + self->sizex;
	}
	if (y < 0) {
		y = y + self->sizey;
	}

	return self->grid[x][y];
}

uint64_t xorshift(void) {
	uint64_t x = s[0];
	uint64_t const y = s[1];
	s[0] = y;
	x ^= x << 23;
	x ^= x >> 17;
	x ^= y ^ (y >> 26);
	s[1] = x;
	return x + y;
}
