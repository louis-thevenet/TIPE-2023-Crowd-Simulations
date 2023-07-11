#include "person.h"
#include <stdio.h>
#include <stdlib.h>


int get_y(int i);
int get_x(int i);


int line_to_grid_x(int i, int height);
int line_to_grid_y(int i, int height);

int heur_eucl_pop(int y, int x, location goal, map *m);


int a_star(map *m, location start, location goal);


void move_basic(map *m, person *p, int next);
void move_stress(map *m, person **p, int pop, int *pred, int p0);



