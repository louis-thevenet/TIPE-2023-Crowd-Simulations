#include "person.h"

#define RANDOM_SPAWN 0

person **generate_population(map *m, int n) {
  person **res = malloc(sizeof(person) * n);

  for (int i = 0; i < n; i++) {
    res[i] = malloc(sizeof(person));

    if (RANDOM_SPAWN && rand() % 10 < 4) {
      res[i]->pos.x = -1;
      res[i]->pos.y = -1;
      while (res[i]->pos.x == -1 || res[i]->pos.y == -1 ||
             m->level[res[i]->pos.y][res[i]->pos.x] != Air) {

        res[i]->pos.x = rand() % m->width;
        res[i]->pos.y = rand() % m->height;
      }
    } else {
      res[i]->pos = m->starts[rand() % (m->start_nb)];
    }

    res[i]->goal = m->exits[rand() % (m->exit_nb)];
  }
  return res;
}

void free_population(person **p, int n) {
  for (int i = 0; i < n; i++) {
    free(p[i]);
  }
  free(p);
}