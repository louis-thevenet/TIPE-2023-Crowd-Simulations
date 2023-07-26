#include "image.h"

void simulation(char *map_name, int initial_pop, int scale)
{

  map *m = load_map(map_name);
  person **p = generate_population(m, initial_pop);
  int *pred = malloc(sizeof(int) * initial_pop);
  int pop = initial_pop;
  int cnt = initial_pop;
  int j = 0;
  double aver_dens = 0;

  while (cnt != 0)
  {
    cnt = pop;

    for (int i = 0; i < pop; i++)
    {
      pred[i] = a_star(m, p[i]->pos, p[i]->goal);
      move_stress(m, p, pop, pred, i);

      if (p[i]->pos.y == p[i]->goal.y && p[i]->pos.x == p[i]->goal.x)
      {
        cnt--;
      }
    }
    save_image(j, create_image(m, p, pop, scale), m->width, m->height);

    printf("\r#%d, %dp, %fp/m^2", j, cnt, (float)cnt / (float)m->surface);
    fflush(stdout);
    aver_dens += (float)cnt / (float)m->surface;
    j++;
    if (j > 300)
    {
      printf("Simulation exceeded 300 steps\n");
      return;
    }
  }

  printf("average density : %f\n", aver_dens / (double)j);
  free_population(p, initial_pop);
  free(pred);
  free_map(m);
}

int main(int argc, char **argv)
{
  if (argc <= 3 || atoi(argv[2]) == 0)
  {
    printf("Missing parameters");
    return 1;
  }
  simulation(argv[1], atoi(argv[2]), atoi(argv[3]));
  return 0;
}
