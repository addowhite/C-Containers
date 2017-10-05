#include <stdlib.h>
#include <stdio.h>
#include "test_grid.h"
#include "grid.h"
#include "iter.h"
#include "utils.h"

static uint test_grid_create(void) {
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

static uint test_grid_set(void) {
  uint width = 100, height = 100;

  int data[100][100];
  for range(x, 0, width - 1)
    for range(y, 0, height - 1)
      data[x][y] = rand() % 2000 - 1000;

  Grid *grid = grid_create(width, height);

  for range(x, 0, width - 1)
    for range(y, 0, height - 1)
      if (!grid_set(grid, x, y, &data[x][y]))
        return test_failed("test_grid_set", "Failed to set grid value", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (grid_get(grid, width, 0) != NULL)
    return test_failed("test_grid_set", "Attempt to retrieve out of bounds value apparently successful", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (grid_get(grid, 0, height) != NULL)
    return test_failed("test_grid_set", "Attempt to retrieve out of bounds value apparently successful", __FILE__, __LINE__);

  for range(x, 0, width - 1)
    for range(y, 0, height - 1)
      if (*(int *)grid_get(grid, x, y) != data[x][y])
        return test_failed("test_grid_set", "Incorrect value retrieved after setting grid values", __FILE__, __LINE__);

  grid_destroy(grid);
  return SUCCESS;
}

static uint test_grid_shift(void) {
  uint width = 100, height = 100;

  int data[100][100];
  for range(x, 0, width - 1)
    for range(y, 0, height - 1)
      data[x][y] = rand() % 10;

  Grid *grid = grid_create(width, height);

  for range(shift_offset_y, 10, -10) {
    for range(shift_offset_x, 10, -10) {

      // Set the data
      for range(x, 0, width - 1)
        for range(y, 0, height - 1)
          if (!grid_set(grid, x, y, &data[x][y]))
            return test_failed("test_grid_shift", "Failed to set grid value", __FILE__, __LINE__);

      // Shift the data
      if (!grid_shift(grid, shift_offset_x, shift_offset_y))
        return test_failed("test_grid_shift", "Failed to shift grid", __FILE__, __LINE__);

      // Check the shifted values are correct
      for range(x, max(0, shift_offset_x), width - 1 + min(0, shift_offset_x))
        for range(y, max(0, shift_offset_y), height - 1 + min(0, shift_offset_y))
          if ((int *)grid_get(grid, x, y) != &data[x - shift_offset_x][y - shift_offset_y])
            return test_failed("test_grid_shift", "Value incorrect after grid shift", __FILE__, __LINE__);

      // Check the values outside the shift have been cleared
      if (shift_offset_y != 0)
        for range(y, ((shift_offset_y > 0) ? 0 : height + shift_offset_y), ((shift_offset_y > 0) ? shift_offset_y - 1 : height - 1))
          for range(x, 0, width - 1)
            if (grid_get(grid, x, y) != NULL)
              return test_failed("test_grid_shift", "Value not null after shift", __FILE__, __LINE__);
      if (shift_offset_x != 0)
        for range(y, 0, height - 1)
          for range(x, ((shift_offset_x > 0) ? 0 : width + shift_offset_x), ((shift_offset_x > 0) ? shift_offset_x - 1 : width - 1))
            if (grid_get(grid, x, y) != NULL)
              return test_failed("test_grid_shift", "Value not null after shift", __FILE__, __LINE__);
    }
  }
}

uint test_grid(void) {
  printf("\nTesting grid...\n");

  CHECK_TEST(test_grid_create());
  CHECK_TEST(test_grid_set());
  CHECK_TEST(test_grid_shift());

  printf("All grid tests passed!\n");
  return SUCCESS;
}
