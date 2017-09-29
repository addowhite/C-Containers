#include <stdlib.h>
#include <stdio.h>
#include "test_grid.h"
#include "grid.h"

uint test_grid_create(void) {
  Grid *grid = grid_create(10, 5);

  if (grid == NULL)
    return test_failed("test_grid_create", "Created grid was NULL", __FILE__, __LINE__);

  if (grid_get_width(grid) != 10)
    return test_failed("test_grid_create", "Initial width was incorrect", __FILE__, __LINE__);

  if (grid_get_height(grid) != 5)
    return test_failed("test_grid_create", "Initial height was incorrect", __FILE__, __LINE__);

  grid_destroy(grid);

  grid = grid_create(5, 10);

  if (grid == NULL)
    return test_failed("test_grid_create", "Created grid was NULL", __FILE__, __LINE__);

  if (grid_get_width(grid) != 5)
    return test_failed("test_grid_create", "Initial width was incorrect", __FILE__, __LINE__);

  if (grid_get_height(grid) != 10)
    return test_failed("test_grid_create", "Initial height was incorrect", __FILE__, __LINE__);

  grid_destroy(grid);
  return SUCCESS;
}

uint test_grid_set(void) {
  uint width = 100, height = 100;

  int data[100][100];
  for (uint y = 0; y < height; ++y)
    for (uint x = 0; x < width; ++x)
      data[y][x] = rand() % 2000 - 1000;

  Grid *grid = grid_create(width, height);

  for (uint y = 0; y < height; ++y)
    for (uint x = 0; x < width; ++x)
      if (!grid_set(grid, x, y, &data[y][x]))
        return test_failed("test_grid_set", "Failed to set grid value", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (grid_get(grid, width, 0) != NULL)
    return test_failed("test_grid_set", "Attempt to retrieve out of bounds value apparently successful", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (grid_get(grid, 0, height) != NULL)
    return test_failed("test_grid_set", "Attempt to retrieve out of bounds value apparently successful", __FILE__, __LINE__);

  int test = 5;
  grid_set(grid, 55, 55, &test);

  for (uint y = 0; y < height; ++y)
    for (uint x = 0; x < width; ++x)
      if (*(int *)grid_get(grid, x, y) != data[y][x])
        return test_failed("test_grid_set", "Incorrect value retrieved after setting grid values", __FILE__, __LINE__);

  grid_destroy(grid);
  return SUCCESS;
}

uint test_grid(void) {
  printf("\nTesting grid...\n");

  CHECK_TEST(test_grid_create());
  CHECK_TEST(test_grid_set());

  printf("All grid tests passed!\n");
  return SUCCESS;
}
