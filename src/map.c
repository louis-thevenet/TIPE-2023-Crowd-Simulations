#include "map.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DIAG 0

map *load_map(char *filename) {
    FILE *f = fopen(filename, "r");
    struct map *m = (struct map *)malloc(1 * sizeof(struct map));
    m->name = (char *)malloc(40 * sizeof(char));
    fscanf(f, "%s\n", m->name);
    fscanf(f, "%d", &(m->width));
    fscanf(f, "%d\n", &(m->height));
    fscanf(f, "%d", &(m->start_nb));
    fscanf(f, "%d\n", &(m->exit_nb));

    m->starts = malloc(sizeof(location) * m->start_nb);
    int start_cnt = 0;

    m->exits = malloc(sizeof(location) * m->exit_nb);
    int exit_cnt = 0;

    m->level = (int **)malloc((m->height) * sizeof(int *));
    m->vert_nb = 0;
    for (int i = 0; i < m->height; i++) {
        m->level[i] = (int *)malloc((m->width) * sizeof(int));
        for (int j = 0; j < m->width; j++) {
            fscanf(f, "%d", &(m->level[i][j]));

            if (m->level[i][j] != Wall)
                m->vert_nb++;
            else
                m->surface++; // un carré = 1m²

            if (m->level[i][j] == Start) {
                if (start_cnt >= m->start_nb) {
                    printf("Found too many starts, %d were specified",
                           m->start_nb);
                    exit(0);
                }
                m->starts[start_cnt] = (location){i, j};
                start_cnt++;
            }

            if (m->level[i][j] == Exit) {
                if (exit_cnt >= m->exit_nb) {
                    printf("Found too many exits, %d were specified",
                           m->exit_nb);
                    exit(0);
                }
                m->exits[exit_cnt] = (location){i, j};
                exit_cnt++;
            }
        }
    }
    // adj. matrix of the graph
    m->adj = malloc(sizeof(bool *) * (m->height * m->width));
    for (int i = 0; i < m->height; i++) {
        for (int j = 0; j < m->width; j++) {
            if (m->level[i][j] != Wall) {
                m->adj[m->width * i + j] = malloc(sizeof(bool) * 8);
                get_neighbours(m, i, j, m->adj[m->width * i + j]);
            } else {
                m->adj[m->width * i + j] = NULL;
            }
        }
    }
    return m;
}

void get_neighbours(map *m, int y, int x, bool *adj)
/*
 * Return
 * neighbours
 * of
 * position
 * (y,x)
 * for
 * adj.
 * matrix
 * */
{
    /*
    0 1
    2 3
    (x,y)
    4 5
    6 7
    */

    if (DIAG == 1) {
        adj[0] = ((x != 0) && y != 0 && m->level[y - 1][x - 1] != Wall) ? 1 : 0;
        adj[1] = (y != 0 && m->level[y - 1][x] != Wall) ? 1 : 0;
        adj[2] = (y != 0 && x != m->width - 1 && m->level[y - 1][x + 1] != Wall)
                     ? 1
                     : 0;
        adj[3] = (x != 0 && m->level[y][x - 1] != Wall) ? 1 : 0;
        adj[4] = (x != m->width - 1 && m->level[y][x + 1] != Wall) ? 1 : 0;
        adj[5] =
            (x != 0 && y != m->height - 1 && m->level[y + 1][x - 1] != Wall)
                ? 1
                : 0;
        adj[6] = (y != m->height - 1 && m->level[y + 1][x] != Wall) ? 1 : 0;
        adj[7] = (x != m->width - 1 && y != m->height - 1 &&
                  m->level[y + 1][x + 1] != Wall)
                     ? 1
                     : 0;
    } else {
        adj[0] = 0;
        adj[1] = (y != 0 && m->level[y - 1][x] != Wall) ? 1 : 0;
        adj[2] = 0;
        adj[3] = (x != 0 && m->level[y][x - 1] != Wall) ? 1 : 0;
        adj[4] = (x != m->width - 1 && m->level[y][x + 1] != Wall) ? 1 : 0;
        adj[5] = 0;
        adj[6] = (y != m->height - 1 && m->level[y + 1][x] != Wall) ? 1 : 0;
        adj[7] = 0;
    }
}

void free_map(map *m) {
    for (int i = 0; i < m->height; i++) {
        free(m->level[i]);
    }
    free(m->level);

    for (int i = 0; i < m->vert_nb; i++) {

        free(m->adj[i]);
    }
    free(m->adj);
    free(m->name);
    free(m->starts);
    free(m->exits);
    free(m);
}
