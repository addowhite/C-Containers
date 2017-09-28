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
  return SUCCESS;
}

uint test_grid(void) {
  printf("\nTesting grid...\n");

  CHECK_TEST(test_grid_create());

  printf("All grid tests passed!\n");
  return SUCCESS;
}
