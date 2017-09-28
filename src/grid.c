#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iter.h"
#include "vector.h"
#include "grid.h"

struct PrivateGrid {
    IterType type;
    Vector *data;
    uint width, height;
};

Grid * grid_create(uint width, uint height) {
    Grid *new_grid = malloc(sizeof(*new_grid));
    if (!new_grid) {
        fprintf(stderr, "%s: Failed to create grid\n", __FILE__);
        return NULL;
    }
    new_grid->type = ITER_TYPE_GRID;
    new_grid->width = width;
    new_grid->height = height;
    new_grid->data = vector_create();
    if (!new_grid->data) {
        fprintf(stderr, "%s: Failed to create list for grid data\n", __FILE__);
        free(new_grid);
        return NULL;
    }
    if (!vector_resize(new_grid->data, width * height)) {
        fprintf(stderr, "%s, Failed to resize vector when creating grid\n", __FILE__);
        vector_destroy(new_grid->data);
        free(new_grid);
        return NULL;
    }
    return new_grid;
}

uint grid_get_width(Grid *grid) {
    return grid->width;
}

uint grid_get_height(Grid *grid) {
    return grid->height;
}

void * grid_get(Grid *grid, uint x, uint y) {
    if (x >= grid->width || y >= grid->height) {
        fprintf(stderr, "%s: Grid read out of bounds at [%u, %u]\n", __FILE__, x, y);
        return NULL;
    }
    return vector_get(grid->data, y * grid->width + x);
}

uint grid_set(Grid *grid, uint x, uint y, void *value) {
    if (x >= grid->width || y >= grid->height) {
        fprintf(stderr, "%s: Grid write out of bounds at [%u, %u]\n", __FILE__, x, y);
        return ERROR;
    }
    return vector_set(grid->data, y * grid->width + x, value);
}

void grid_destroy(Grid *grid) {
    free(grid);
}
