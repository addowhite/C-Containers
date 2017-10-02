#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "test_iter.h"
#include "iter.h"
#include "vector.h"
#include "grid.h"
#include "hashmap.h"

static char * string_fill_random(char *str, uint count) {
  char base = 'A';
  for (uint i = 0; i < count; ++i)
    str[i] = base + rand() % 26;
  return str;
}

static uint test_iter_vector(void) {
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

static uint test_iter_grid(void) {
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

static uint test_iter_hashmap(void) {
  HashMap *hashmap = hashmap_create();

  uint value_count = 20;
  char keys[20][11];
  int values[20];
  for (uint i = 0; i < value_count; ++i) {
    values[i] = rand() % 0x11111111;
    string_fill_random(keys[i], 10);
    keys[i][10] = 0;
    hashmap_set(hashmap, keys[i], &values[i]);
  }

  foreach(HashMapValueContainer, hashmap, iter)
    if (hashmap_get(hashmap, iter->key) != iter->value)
      return test_failed("test_iter_hashmap", "Key-value pair incorrect", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

static uint test_iter_range(void) {
  // Test iterating forward
  {
    uint test_index = 0;
    for range(i, 0, 10)
      if (i != test_index++)
        return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

    int test_x = -5, test_y = -5;
    for range(y, -5, 5) {
      for range(x, -5, 5) {
        if (x != test_x)
          return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

        if (y != test_y)
          return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

        if (++test_x > 5) { test_x = -5; test_y++; }
      }
    }
  }

  // Test iterating backward
  {
    uint test_index = 10;
    for range(i, 10, 0)
      if (i != test_index--)
        return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

    int test_x = 5, test_y = 5;
    for range(y, 5, -5) {
      for range(x, 5, -5) {
        if (x != test_x)
          return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

        if (y != test_y)
          return test_failed("test_iter_range", "Index was incorrect when iterating range", __FILE__, __LINE__);

        if (--test_x < -5) { test_x = 5; test_y--; }
      }
    }
  }

  // Test each argument is evaluated exactly once
  {
    int lower = 0, upper = 0, loop_count = 0;
    for range(i, ++lower, ++upper + 4) loop_count++;

    if (lower != 1)
      return test_failed("test_iter_range", "Range lower bound is not evaluated exactly once", __FILE__, __LINE__);

    if (upper != 1)
      return test_failed("test_iter_range", "Range upper bound is not evaluated exactly once", __FILE__, __LINE__);

    if (loop_count != 5)
      return test_failed("test_iter_range", "Incorrect number of iterations in range", __FILE__, __LINE__);
  }

  return SUCCESS;
}

uint test_iter(void) {
	printf("\nTesting iter...\n");

	// Set random seed
	time_t t;
	srand((uint) time(&t));

	CHECK_TEST(test_iter_vector());
	CHECK_TEST(test_iter_grid());
  CHECK_TEST(test_iter_hashmap());
  CHECK_TEST(test_iter_range());

	printf("All iter tests passed!\n");
}