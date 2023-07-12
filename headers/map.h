#include <stdbool.h>

typedef struct location
{
    int y;
    int x;
} location;

enum objects
{
    Air,
    Wall,
    Start,
    Exit,
    Person
};

typedef struct map
{
    char *name;
    int **level;

    int vert_nb;
    bool **adj;

    int start_nb;
    location *starts;

    int exit_nb;
    location *exits;

    int width;
    int height;
    int surface;
} map;

void print_nei(map *m, int y, int x);
void print_map(map *m);
map *load_map(char *filename);
void get_neighbours(map *m, int y, int x, bool *adj);
void free_map(map *m);
