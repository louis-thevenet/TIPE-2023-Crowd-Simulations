#include "map.h"
#include <stdlib.h>
#include <time.h>

typedef struct person {
  location pos;
  location goal;
} person;

person **generate_population(map *m, int n);

void free_population(person **p, int n);