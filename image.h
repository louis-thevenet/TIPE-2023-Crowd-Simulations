#include "move.h"

void draw_digit(int **tab, int digit, int y0, int x0, int a);
void draw_arrows(int **img, person **p, int pop);
int **create_image(map *m, person **p, int pop);
void free_image(int **img, int height);
void save_image(int n, int **img, int width, int height);

