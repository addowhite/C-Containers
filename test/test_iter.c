#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "test_iter.h"
#include "iter.h"
#include "vector.h"
#include "grid.h"

static uint test_iter_vector() {
	Vector *vector = vector_create();
	if (!vector)
    return test_failed("test_iter_vector", "Failed to create vector for test", __FILE__, __LINE__);

  uint value_count = 100;
  int values[100];

  for (uint i = 0; i < value_count; ++i) {
  	values[i] = rand() % 0x11111111;
		if (!vector_push_back(vector, &values[i]))
	    return test_failed("test_iter_vector", "Failed to failed to push value to vector for test", __FILE__, __LINE__);
  }

  uint i = 0;
  foreach(int, vector, iter) {
  	if (iter != &values[i++])
	    return test_failed("test_iter_vector", "Value read when iterating was incorrect", __FILE__, __LINE__);
	}

	vector_destroy(vector);
	return SUCCESS;
}

static uint test_iter_grid() {
	Grid *grid = grid_create(100, 100);
	if (!grid)
    return test_failed("test_iter_grid", "Failed to create grid for test", __FILE__, __LINE__);

  int values[100][100];

  for (uint y = 0; y < grid_get_height(grid); ++y) {
  	for (uint x = 0; x < grid_get_width(grid); ++x) {
  		values[y][x] = rand() % 0x11111111;
  		grid_set(grid, x, y, &values[y][x]);
  	}
  }

  uint x = 0, y = 0;
  foreach(int, grid, iter) {
  	if (*iter != *(int *)grid_get(grid, x, y))
  		return test_failed("test_iter_grid", "Incorrect value found when iterating grid", __FILE__, __LINE__);
  	if (++x >= grid_get_width(grid)) { x = 0; y++; };
  }

	grid_destroy(grid);
	return SUCCESS;
}

uint test_iter(void) {
	printf("\nTesting iter...\n");

	// Set random seed
	time_t t;
	srand((uint) time(&t));

	CHECK_TEST(test_iter_vector());
	CHECK_TEST(test_iter_grid());

	printf("All iter tests passed!\n");
}