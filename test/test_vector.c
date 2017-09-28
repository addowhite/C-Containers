#include <stdio.h>
#include "test_vector.h"
#include "vector.h"

static uint test_vector_create(void) {
  Vector *vector = vector_create();

  if (vector == NULL)
    return test_failed("test_vector_create", "Created vector was NULL", __FILE__, __LINE__);

  if (vector_size(vector) != 0)
    return test_failed("test_vector_create", "Initial size was incorrect", __FILE__, __LINE__);

  if (vector_capacity(vector) != 1)
    return test_failed("test_vector_create", "Initial capacity was incorrect", __FILE__, __LINE__);

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_resize() {
  Vector *vector = vector_create();

  if (!vector_resize(vector, 5))
    return test_failed("test_vector_resize", "Failed to resize vector", __FILE__, __LINE__);

  if (vector_size(vector) != 5)
    return test_failed("test_vector_resize", "Size after resize operation incorrect", __FILE__, __LINE__);

  if (vector_capacity(vector) != 7)
    return test_failed("test_vector_resize", "Capacity after resize operation incorrect", __FILE__, __LINE__);

  if (!vector_resize(vector, 100))
    return test_failed("test_vector_resize", "Failed to resize vector", __FILE__, __LINE__);

  if (vector_size(vector) != 100)
    return test_failed("test_vector_resize", "Size after resize operation incorrect", __FILE__, __LINE__);

  if (vector_capacity(vector) != 150)
    return test_failed("test_vector_resize", "Capacity after resize operation incorrect", __FILE__, __LINE__);

  if (!vector_resize(vector, 100))
    return test_failed("test_vector_resize", "Failed to resize vector", __FILE__, __LINE__);

  if (vector_size(vector) != 100)
    return test_failed("test_vector_resize", "Size after resize operation incorrect", __FILE__, __LINE__);

  if (vector_capacity(vector) != 150)
    return test_failed("test_vector_resize", "Capacity after resize operation incorrect", __FILE__, __LINE__);

  for (uint i = 0; i < vector_size(vector); ++i)
    if (vector_get(vector, i) != NULL)
      return test_failed("test_vector_resize", "Values not correctly initialised", __FILE__, __LINE__);

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_push_back(void) {
  Vector *vector = vector_create();

  uint value_count = 5;
  int values[] = { 100, 101, 0, 0x11111111, 69 };
  for (uint i = 0; i < value_count; ++i) {
    if (!vector_push_back(vector, &values[i]))
      return test_failed("test_vector_push_back", "Failed to push value to vector", __FILE__, __LINE__);

    fprintf(stderr, "Expecting read out of bounds for test...\n");
    if (vector_get(vector, i + 1) != NULL)
      return test_failed("test_vector_push_back", "Value was not NULL during deliberate read out of bounds", __FILE__, __LINE__);
  }

  if (vector_size(vector) != value_count)
    return test_failed("test_vector_push_back", "Vector size incorrect after pushing values", __FILE__, __LINE__);

  for (uint i = 0; i < value_count; ++i) {
    int *current_value = vector_get(vector, i);

    if (current_value == NULL)
      return test_failed("test_vector_push_back", "Pushed value was read back as NULL", __FILE__, __LINE__);

    if (*current_value != values[i])
      return test_failed("test_vector_push_back", "Pushed value was read back with incorrect value", __FILE__, __LINE__);
  }

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_set(void) {
  Vector *vector = vector_create();

  int values[] = { 0, -1, 100, -101, 0x11111111 };

  vector_resize(vector, 5);
  for (uint i = 0; i < vector_size(vector); ++i)
    if (!vector_set(vector, i, &values[i]))
      return test_failed("test_vector_set", "Failed to set value", __FILE__, __LINE__);

  fprintf(stderr, "Expecting write out of bounds for test...\n");
  if (vector_get(vector, 6) != NULL)
    return test_failed("test_vector_set", "Attempted deliberate write out of bounds was supposedly successful", __FILE__, __LINE__);

  for (uint i = 0; i < vector_size(vector); ++i)
    if (*(int *)vector_get(vector, i) != values[i])
      return test_failed("test_vector_set", "Incorrect value found after settings values", __FILE__, __LINE__);

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_insert(void) {
  Vector *vector = vector_create();

  int values[] = { 0x11111111, 0, 7, 999, 12345 };
  for (uint i = 0; i < 5; ++i)
    vector_push_back(vector, &values[i]);

  int new_indices[] = { 0, 2, 7 };
  int new_values[] = { 3, 2, 77 };
  for (uint i = 0; i < 3; ++i)
    if (!vector_insert(vector, new_indices[i], &new_values[i]))
      return test_failed("test_vector_insert", "Failed to insert value", __FILE__, __LINE__);

  int final_values[] = { 3, 0x11111111, 2, 0, 7, 999, 12345, 77 };
  for (uint i = 0; i < 8; ++i)
    if (*(int *)vector_get(vector, i) != final_values[i])
      return test_failed("test_vector_insert", "Incorrect value found after inserting values", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (vector_get(vector, 8) != NULL)
    return test_failed("test_vector_insert", "Value was not NULL during deliberate read out of bounds", __FILE__, __LINE__);

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_find_index(void) {
  Vector *vector = vector_create();

  int values[] = { 3, 0x11111111, 2, 0, 7, 999, 12345, 77 };
  for (uint i = 0; i < 8; ++i)
    vector_push_back(vector, &values[i]);

  uint find_indices[] = { 0, 2, 7 };
  for (uint i = 0; i < 3; ++i)
    if (vector_find_index(vector, &values[find_indices[i]]) != find_indices[i])
      return test_failed("test_vector_find_index", "Incorrect index returned", __FILE__, __LINE__);

  int num = 6;
  if (vector_find_index(vector, &num) != -1 || vector_find_index(vector, NULL) != -1)
    return test_failed("test_vector_find_index", "Search for non-existant value unexpectedly did not fail", __FILE__, __LINE__);

  vector_destroy(vector);
  return SUCCESS;
}

static uint test_vector_push_vector(void) {
  int values[] = { 0, -1, 100, -101, 0x11111111, -99999, 345215, 21, -21, 2 };

  Vector *vector_a = vector_create(), *vector_b = vector_create();
  for (uint i = 0; i < 10; ++i)
    vector_push_back((i < 5 ? vector_a : vector_b), &values[i]);

  if (!vector_push_vector(vector_a, vector_b))
    return test_failed("test_vector_push_vector", "Failed to push vector to another vector", __FILE__, __LINE__);

  for (uint i = 0; i < 10; ++i)
    if (*(int *)vector_get(vector_a, i) != values[i])
      return test_failed("test_vector_push_vector", "Incorrect value returned after pushing vector to another vector", __FILE__, __LINE__);

  fprintf(stderr, "Expecting read out of bounds for test...\n");
  if (vector_get(vector_a, 10) != NULL)
    return test_failed("test_vector_push_vector", "Value was not NULL during deliberate read out of bounds", __FILE__, __LINE__);

  vector_destroy(vector_a);
  vector_destroy(vector_b);
  return SUCCESS;
}

static uint test_vector_clear(void) {
  Vector *vector = vector_create();

  int values[] = { 0, -1, 100, -101, 0x11111111, -99999, 345215, 21, -21, 2 };
  for (uint i = 0; i < 10; ++i)
    vector_push_back(vector, &values[i]);

  if (!vector_clear(vector))
    return test_failed("test_vector_clear", "Failed to clear vector", __FILE__, __LINE__);

  if (vector_size(vector) != 0)
    return test_failed("test_vector_clear", "Incorrect size after clearing vector", __FILE__, __LINE__);

  if (vector_capacity(vector) != 16)
    return test_failed("test_vector_clear", "Capacity size after clearing vector", __FILE__, __LINE__);

  for (uint i = 0; i < 10; ++i) {
    fprintf(stderr, "Expecting read out of bounds for test...\n");
    if (vector_get(vector, i) != NULL)
      return test_failed("test_vector_clear", "Value was not NULL after clearing vector", __FILE__, __LINE__);
  }

  vector_destroy(vector);
  return SUCCESS;
}

uint test_vector(void) {
  printf("\nTesting vector...\n");

  CHECK_TEST(test_vector_create());
  CHECK_TEST(test_vector_resize());
  CHECK_TEST(test_vector_push_back());
  CHECK_TEST(test_vector_set());
  CHECK_TEST(test_vector_insert());
  CHECK_TEST(test_vector_find_index());
  CHECK_TEST(test_vector_push_vector());
  CHECK_TEST(test_vector_clear());

  printf("All vector tests passed!\n");
  return SUCCESS;
}