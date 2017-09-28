#ifndef _GRID_H_
#define _GRID_H_

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#define ERROR 0
#define NO_ERROR 1

struct PrivateGrid;
typedef struct PrivateGrid Grid;

Grid * grid_create(uint width, uint height);
uint grid_get_width(Grid *grid);
uint grid_get_height(Grid *grid);
void * grid_get(Grid *grid, uint x, uint y);
uint grid_set(Grid *grid, uint x, uint y, void *value);
void grid_destroy();

#endif // _GRID_H_