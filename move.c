#include "min_heap.h"
#include "person.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define INT_MAX 100000
#define MAX_PERSON 6
#define MAX_PERSON_REGULAR 3
#define HEUR heur_eucl_pop
#define FORCE_REPULSION 4

int get_y(int i) {
    if (i < 3)
        return -1;
    if (i > 4)
        return 1;
    return 0;
}

int get_x(int i) {
    if (i == 0 || i == 3 || i == 5)
        return -1;
    if (i == 2 || i == 4 || i == 7)
        return 1;
    return 0;
}

int line_to_grid_x(int i, int width) { return i % width; }

int line_to_grid_y(int i, int width) { return (i - i % width) / width; }

int heur_eucl(int y, int x, location goal, map *m) {
    return sqrt((goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y));
}

int heur_eucl_pop(int y, int x, location goal, map *m) { // prend en compte les gens
    int f = 1;
    for (int i=0; i<FORCE_REPULSION; i++) {
        f*=m->level[y][x];
    }
    return ((goal.x - x) * (goal.x - x) + (goal.y - y) * (goal.y - y)+f);
}

int a_star(map *m, location start, location goal) {
    int *pred = malloc(sizeof(int) * (m->height * m->width));
    int *g = malloc(sizeof(int) * m->height * m->width);
    int *f = malloc(sizeof(int) * m->height * m->width);

    int c, u, x_u, y_u, v, x_v, y_v, g_score;

    Heap *Q = CreateHeap(m->height * m->width, 0);

    for (int i = 0; i < m->height * m->width; i++) {
        g[i] = INT_MAX;
        f[i] = INT_MAX;
        pred[i] = -1;
        if (start.y * m->width + start.x == i) {
            g[start.y * m->width + start.x] = 0;
            f[start.y * m->width + start.x] = 0 + HEUR(start.y, start.x, goal, m);
        }

        if (m->level[line_to_grid_y(i, m->width)][line_to_grid_x(i, m->width)] != Wall) {
            insert(Q, i, f);
        }
    }

    while (Q->count != 0) {
        u = PopMin(Q, f);
        if (u == goal.y * m->width + goal.x) {
            while (pred[u] != start.y * m->width + start.x && u != -1) {
                u = pred[u];
            }
            free(g);
            free(f);
            free(pred);
            FreeHeap(Q);
            return u;
        }

        x_u = line_to_grid_x(u, m->width);
        y_u = line_to_grid_y(u, m->width);
        if (m->adj[u] != NULL) { // pas un mur (au cas où)
            bool *adj = m->adj[u];
            for (int i = 0; i < 8; i++) {
                if (adj[i] == true) {
                    y_v = y_u + get_y(i);
                    x_v = x_u + get_x(i);
                    v = y_v * m->width + x_v;
                    c = 1;
                    g_score = g[u] + c;


                    if (g_score < g[v]) {

                        g[v] = g_score;
                        f[v] = g_score + HEUR(y_v, x_v, goal, m);
                        pred[v] = u;
                        heapify_bottom_top(Q, Q->pos[y_v * m->width + x_v], f);
                    }
                }
            }
        }
    }
    free(g);
    free(f);
    free(pred);
    FreeHeap(Q);
    return -1;
}

void move_basique(map *m, person *p, int next) {
    if (next == -1) return;
    int y = line_to_grid_y(next, m->width);
    int x = line_to_grid_x(next, m->width);

    if (m->level[y][x] >= Person + MAX_PERSON - 1) return; //on peut pas bouger ici car trop de gens

    if (m->level[p->pos.y][p->pos.x] != Start && m->level[p->pos.y][p->pos.x] != Exit) {
        m->level[p->pos.y][p->pos.x] = (m->level[p->pos.y][p->pos.x] > Person) ? m->level[p->pos.y][p->pos.x] - 1 : Air;
    }
    p->pos = (location) {y, x};
    if (p->goal.y == y && p->goal.x == x) return;

    if (m->level[y][x] != Start && m->level[y][x] != Exit) {
        m->level[y][x] = (m->level[y][x] >= Person) ? m->level[y][x] + 1 : Person;
    }
}

void move_stress(map *m, person **p, int pop, int *pred, int p0) {

    if (pred[p0] == -1) return;
    int y = line_to_grid_y(pred[p0], m->width);
    int x = line_to_grid_x(pred[p0], m->width);

    if (m->level[y][x] >= Person + MAX_PERSON_REGULAR - 1) { // trop de gens pour vouloir y aller en temps normal
        int cnt=0;
        for (int i=p0; i<pop; i++) {
            if (pred[i]!=-1 && line_to_grid_y(pred[i], m->width)==p[i]->pos.y
                        && line_to_grid_x(pred[i], m->width)==p[i]->pos.x) cnt++;
        }
        if (cnt<2) { //pas assez de gens poussent pour y aller
            return;
        }
    }
    if (m->level[y][x] >= Person + MAX_PERSON - 1) return; //on peut pas bouger ici car trop de gens

    if (m->level[p[p0]->pos.y][p[p0]->pos.x] != Start && m->level[p[p0]->pos.y][p[p0]->pos.x] != Exit) {

        m->level[p[p0]->pos.y][p[p0]->pos.x] = (m->level[p[p0]->pos.y][p[p0]->pos.x] > Person) ?
                                                        m->level[p[p0]->pos.y][p[p0]->pos.x] - 1 : Air;
    }
    p[p0]->pos = (location) {y, x};
    if (p[p0]->goal.y == y && p[p0]->goal.x == x) return;

    if (m->level[y][x] != Start && m->level[y][x] != Exit) {
        m->level[y][x] = (m->level[y][x] >= Person) ? m->level[y][x] + 1 : Person;
    }
    pred[p0]=-1;
}