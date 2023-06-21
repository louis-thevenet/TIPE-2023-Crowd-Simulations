#include "image.h"
#define POP 60

void simulation(char *map_name) {
  map *m = load_map(map_name);
  person **p = generate_population(m, POP);
  int *pred = malloc(sizeof(int) * POP);
  int pop = POP;
  int cnt = POP;
  int j = 0;
  double aver_dens = 0;

  while (cnt != 0) {
    cnt = pop;

    for (int i = 0; i < pop; i++) {
      pred[i] = a_star(m, p[i]->pos, p[i]->goal);
      move_stress(m, p, pop, pred, i);

      if (p[i]->pos.y == p[i]->goal.y && p[i]->pos.x == p[i]->goal.x) {
        cnt--;
      }
    }
    save_image(j, create_image(m, p, pop), m->width, m->height);

    printf("#%d, %dp, %fp/m^2\n", j, cnt, (float)cnt / (float)m->surface);
      aver_dens += (float)cnt / (float)m->surface;
    j++;
    if (j > 300)
      return;
  }

  printf("average density : %f\n", aver_dens / (double)j);
  free_population(p, POP);
  free(pred);
  free_map(m);
}

int main(int argc, char **argv) {
  if (argc <= 1) return 1;
    simulation(argv[1]);
  return 0;
}
