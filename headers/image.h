#include "move.h"

typedef struct struct_image {
  int **pixels;
  int scale;
} image;

void draw_digit(image *img, int digit, int y0, int x0, int a);
void draw_arrows(image *img, person **p, int pop);
image *create_image(map *m, person **p, int pop, int scale);
void free_image(image *img, int height);
void save_image(int n, image *img, int width, int height);