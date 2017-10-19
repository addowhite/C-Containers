#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iter.h"
#include "vector.h"
#include "grid.h"
#include "utils.h"

struct PrivateGrid {
  IterType type;
  Vector *linearized;
  Vector *data;
  uint width, height;
};

Grid * grid_create(uint width, uint height) {
  Grid *new_grid = malloc(sizeof(*new_grid));
  if (!new_grid) {
    fprintf(stderr, "%s: Failed to create grid\n", __FILE__);
    return NULL;
  }
  new_grid->type   = ITER_TYPE_GRID;
  new_grid->width  = width;
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

uint grid_shift(Grid *grid, int offset_x, int offset_y) {
  int width = grid_get_width(grid), height = grid_get_height(grid);

  if (offset_y != 0) {
    if (!vector_move(grid->data, 0, grid->data, offset_y * width, width * height)) {
      fprintf(stderr, "%s: Failed to move vector data when shifting grid\n", __FILE__);
      return ERROR;
    }
  }

  if (offset_x != 0) {
    if (!vector_move(grid->data, max(0, -offset_x), grid->data, max(0, offset_x), width * height)) {
      fprintf(stderr, "%s: Failed to move vector data when shifting grid\n", __FILE__);
      return ERROR;
    }

    for range(y, 0, height - 1)
      vector_erase(grid->data, y * width + ((offset_x > 0) ? 0 : width + offset_x), abs(offset_x));
  }
  return NO_ERROR;
}

uint grid_read_from_file(Grid *grid, uint element_size, FILE *file) {
  fread(&grid->width,  sizeof(grid->width),  1, file);
  fread(&grid->height, sizeof(grid->height), 1, file);
  vector_read_from_file(grid->data, element_size, file);
  return NO_ERROR;
}

uint grid_write_to_file(Grid *grid, uint element_size, FILE *file) {
  fwrite(&grid->width,  sizeof(grid->width),  1, file);
  fwrite(&grid->height, sizeof(grid->height), 1, file);
  vector_write_to_file(grid->data, element_size, file);
  return NO_ERROR;
}

void grid_destroy(Grid *grid) {
  vector_destroy(grid->data);
  free(grid);
  grid = NULL;
}
