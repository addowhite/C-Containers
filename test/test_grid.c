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
      data[x][y] = rand() % 0x11111111;

  Grid *grid = grid_create(width, height);

  if (!grid)
    return test_failed("test_grid_shift", "Failed to create grid for test", __FILE__, __LINE__);

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
      for (int x = max(0, shift_offset_x); x < width + min(0, shift_offset_x); ++x)
        for (int y = max(0, shift_offset_y); y < height + min(0, shift_offset_y); ++y)
          if ((int *)grid_get(grid, x, y) != &data[x - shift_offset_x][y - shift_offset_y])
            return test_failed("test_grid_shift", "Value incorrect after grid shift", __FILE__, __LINE__);

      // Check the values outside the shift have been cleared
      if (shift_offset_y != 0)
        for (int y = ((shift_offset_y > 0) ? 0 : height + shift_offset_y); y < ((shift_offset_y > 0) ? shift_offset_y : height); ++y)
        // for range(y, ((shift_offset_y > 0) ? 0 : height + shift_offset_y), ((shift_offset_y > 0) ? shift_offset_y - 1 : height - 1))
          for range(x, 0, width - 1)
            if (grid_get(grid, x, y) != NULL)
              return test_failed("test_grid_shift", "Value not null after shift", __FILE__, __LINE__);
      if (shift_offset_x != 0)
        for range(y, 0, height - 1)
          for (int x = ((shift_offset_x > 0) ? 0 : width + shift_offset_x); x < ((shift_offset_x > 0) ? shift_offset_x : width); ++x)
          // for range(x, ((shift_offset_x > 0) ? 0 : width + shift_offset_x), ((shift_offset_x > 0) ? shift_offset_x - 1 : width - 1))
            if (grid_get(grid, x, y) != NULL)
              return test_failed("test_grid_shift", "Value not null after shift", __FILE__, __LINE__);

    }
  }
  grid_destroy(grid);
  return SUCCESS;
}

static uint test_grid_write_to_file(void) {
  Grid *grid_write = grid_create(4, 6);
  Grid *grid_read  = grid_create(0, 0);

  if (!grid_write)
    return test_failed("test_grid_write_to_file", "Failed to create grid for test", __FILE__, __LINE__);

  if (grid_read == NULL)
    return test_failed("test_grid_write_to_file", "Failed to create grid for test", __FILE__, __LINE__);

  int data[24], i = 0;
  for range(y, 0, grid_get_height(grid_write) - 1) {
    for range(x, 0, grid_get_width(grid_write) - 1) {
      data[i] = rand() % 0x11111111;
      grid_set(grid_write, x, y, data + i++);
    }
  }

  FILE *file = fopen("grid_write_test", "w+b");

  if (!file)
    return test_failed("test_grid_write_to_file", "Failed to open file for writing", __FILE__, __LINE__);

  if (!grid_write_to_file(grid_write, sizeof(int), file))
    return test_failed("test_grid_write_to_file", "Failed to write grid data to file", __FILE__, __LINE__);
  fclose(file);

  file = fopen("grid_write_test", "rb");
  if (!grid_read_from_file(grid_read, sizeof(int), file))
    return test_failed("test_grid_write_to_file", "Failed to read grid data from file", __FILE__, __LINE__);
  fclose(file);

  if (grid_get_width(grid_write) != grid_get_width(grid_read))
    return test_failed("test_grid_write_to_file", "Grid width incorrect after writing to file and reading back again", __FILE__, __LINE__);

  if (grid_get_height(grid_write) != grid_get_height(grid_read))
    return test_failed("test_grid_write_to_file", "Grid height incorrect after writing to file and reading back again", __FILE__, __LINE__);

  for range(y, 0, grid_get_height(grid_read) - 1)
    for range(x, 0, grid_get_width(grid_read) - 1)
      if (*(int *)grid_get(grid_read, x, y) != *(int *)grid_get(grid_write, x, y))
        return test_failed("test_grid_write_to_file", "Value incorrect after writing to file and reading back again", __FILE__, __LINE__);

  grid_destroy(grid_read);
  grid_destroy(grid_write);
  return SUCCESS;
}

uint test_grid(void) {
  printf("\nTesting grid...\n");

  CHECK_TEST(test_grid_create());
  CHECK_TEST(test_grid_set());
  CHECK_TEST(test_grid_shift());
  CHECK_TEST(test_grid_write_to_file());

  printf("All grid tests passed!\n");
  return SUCCESS;
}
