#include "image.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pthread.h>

#define SCALE 150

typedef struct thread_arg
{
    map *m;
    int **img;
    double *factors;
    int y;
    person **p;
    int pop;
} thread_arg;

const int digits[10][5][5] = {
    // chiffre 0
    {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}},
    // chiffre 1
    {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {0, 1, 1, 1, 0}},
    // chiffre 2
    {
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0}},
    // chiffre 3
    {
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0}},
    // chiffre 4
    {
        {0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0}},
    // chiffre 5
    {
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0}},
    // chiffre 6
    {
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 1, 1, 0}},
    // chiffre 7
    {
        {0, 1, 1, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0}},
    // chiffre 8
    {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 0}},
    // chiffre 9
    {
        {0, 1, 1, 1, 0},
        {1, 0, 0, 0, 1},
        {0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1},
        {0, 1, 1, 1, 1}}};

void draw_digit(int **tab, int digit, int y0, int x0, int a)
{

    int digi_size = 5;
    for (int i = 1; i < a; i++)
    {
        if ((a - i) % digi_size == 0)
        {
            a = a - i;
            break;
        }
    }

    if (digit > 9)
        digit = 9;
    for (int offset_y = 0; offset_y < digi_size; offset_y++)
    {
        for (int offset_x = 0; offset_x < digi_size; offset_x++)
        {

            for (int i = offset_y * (a / digi_size); i < (offset_y + 1) * (a / digi_size); i++)
            {
                for (int j = offset_x * (a / digi_size); j < (offset_x + 1) * (a / digi_size); j++)
                {

                    tab[y0 + i][x0 + j] = digits[digit][offset_y][offset_x];
                }
            }
        }
    }
}

void draw_arrows(int **img, person **p, int pop)
{
    for (int i = 0; i < pop; i++)
    {
        double alpha = ((double)(p[i]->goal.y - p[i]->pos.y)) / ((double)(p[i]->goal.x - p[i]->pos.x));
        double beta = 0;

        int y0 = SCALE * p[i]->pos.y + SCALE / 2;
        int x0 = SCALE * p[i]->pos.x + SCALE / 2;

        int deb = (p[i]->goal.y - p[i]->pos.y > 0 && alpha < 0) || ((p[i]->goal.y - p[i]->pos.y) < 0 && alpha > 0) ? -SCALE / 2 + 1 : 0;

        int fin = (p[i]->goal.y - p[i]->pos.y > 0 && alpha < 0) || ((p[i]->goal.y - p[i]->pos.y) < 0 && alpha > 0) ? 0 : SCALE / 2;

        for (int x = deb; x < fin; x++)
        {
            if (alpha * x + beta > -SCALE / 2 && alpha * x + beta < SCALE / 2)
            {
                img[(int)(-1 + y0 + alpha * x + beta)][x + x0] = 3;
                img[(int)(y0 + alpha * x + beta)][x + x0] = 3;
                img[(int)(1 + y0 + alpha * x + beta)][x + x0] = 3;
            }
        }
    }
}

void *fill_line(void *args)
{
    thread_arg *arg = (thread_arg *)args;

    for (int x = 0; x < arg->m->width; x++)
    {
        if (arg->m->level[arg->y][x] < Person)
        {
            for (int i = ceil((1 - arg->factors[arg->m->level[arg->y][x]]) * SCALE);
                 i < ceil(SCALE * arg->factors[arg->m->level[arg->y][x]]); i++)
            {
                for (int j = ceil((1 - arg->factors[arg->m->level[arg->y][x]]) * SCALE);
                     j < ceil(SCALE * arg->factors[arg->m->level[arg->y][x]]); j++)
                {
                    arg->img[SCALE * arg->y + i][SCALE * x + j] = arg->m->level[arg->y][x];
                }
            }
        }

        else
        {
            draw_digit(arg->img, arg->m->level[arg->y][x] - 3, SCALE * arg->y, SCALE * x, SCALE);
            draw_arrows(arg->img, arg->p, arg->pop);
        }
    }
}

int **create_image(map *m, person **p, int pop)
{

    int height = m->height * SCALE;
    int width = m->width * SCALE;

    int **img = malloc(sizeof(int *) * height);
    for (int i = 0; i < height; i++)
    {
        img[i] = malloc(sizeof(int) * width);
        for (int j = 0; j < width; j++)
        {
            img[i][j] = 0;
        }
    }
    // { Air,   Wall,  Start,  Exit , Person};
    double factors[] = {.95, .95, 0.97, 0.97, .6};

    pthread_t *threads = malloc(sizeof(pthread_t) * height);
    thread_arg *args;
    for (int y = 0; y < m->height; y++)
    {
        args = malloc(sizeof(thread_arg));
        args->m = m;
        args->factors = factors;
        args->img = img;
        args->pop = pop;
        args->p = p;
        args->y = y;
        pthread_create(&threads[y], NULL, fill_line, args);
    }
    for (int i = 0; i < height; i++)
    { // grille
        for (int j = 0; j < width; j++)
        {
            if (i % SCALE == 0 || j % SCALE == 0)
                img[i][j] = 1;
        }
    }
    for (int y = 0; y < m->height; y++)
        pthread_join(threads[y], NULL);
    return img;
}

void free_image(int **img, int height)
{
    for (int i = 0; i < height; i++)
    {
        free(img[i]);
    }
    free(img);
}

void save_image(int n, int **img, int width, int height)
{
    width *= SCALE;
    height *= SCALE;
    char filename[20];
    FILE *file;
    char colors[4][13] = {
        "255 255 255 \0", // air
        "000 000 000 \0", // mur
        "000 255 000 \0", // sortie
        "255 000 000 \0", // entrée

    };
    // mkdir("./ppms", 0777);
    sprintf(filename, "./ppms/step-%03d.ppm", n);
    file = fopen(filename, "w");
    fprintf(file, "P3\n%d %d\n255\n", width, height);
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            fprintf(file, colors[img[j][i]]);
        }
    }
    fclose(file);
    free_image(img, height);
}