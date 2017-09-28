#include <stdio.h>
#include "test_vector.h"
#include "vector.h"

uint test_vector_create(void) {
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

uint test_vector_push_back(void) {
  Vector *vector = vector_create();

  uint value_count = 5;
  int values[] = { 100, 101, 0, 0x11111111, 69 };
  for (uint i = 0; i < value_count; ++i) {
    if (!vector_push_back(vector, &values[i]))
      return test_failed("test_vector_push_back", "Failed to push value to vector", __FILE__, __LINE__);
  }

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

uint test_vector(void) {
  printf("\nTesting vector...\n");

  CHECK_TEST(test_vector_create());
  CHECK_TEST(test_vector_push_back());

  printf("All vector tests passed!\n");
  return SUCCESS;
}
