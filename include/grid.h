#ifndef _GRID_H_
#define _GRID_H_

#include "utils.h"

struct PrivateGrid;
typedef struct PrivateGrid Grid;

Grid * grid_create(uint width, uint height);
uint grid_get_width(Grid *grid);
uint grid_get_height(Grid *grid);
void * grid_get(Grid *grid, uint x, uint y);
uint grid_set(Grid *grid, uint x, uint y, void *value);
uint grid_shift(Grid *grid, int offset_x, int offset_y);
uint grid_read_from_file(Grid *grid, uint element_size, FILE *file);
uint grid_write_to_file(Grid *grid, uint element_size, FILE *file);
void grid_destroy();

#endif // _GRID_H_
