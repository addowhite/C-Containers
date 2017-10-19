#include <stdlib.h>
#include <stdio.h>
#include "px_string.h"
#include "iter.h"
#include "test_utils.h"
#include "test_hashmap.h"
#include "hashmap.h"

static uint test_hashmap_create(void) {
  HashMap *hashmap = hashmap_create(10, 5);

  if (hashmap == NULL)
    return test_failed("test_hashmap_create", "Created hashmap was NULL", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

static uint test_hashmap_set(void) {
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

  char keys_copy[20][11];
  memcpy(keys_copy, keys, 20 * 11);

  for range(i, 0, 19)
    if (hashmap_get(hashmap, keys_copy[i]) != hashmap_get(hashmap, keys[i]))
      return test_failed("test_iter_hashmap", "Key-value pair incorrect", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

static uint test_hashmap_size(void) {
  HashMap *hashmap = hashmap_create();

  uint value_count = 20;
  char keys[20][11];
  int values[20];
  for (uint i = 0; i < value_count / 2; ++i) {
    values[i] = rand() % 0x11111111;
    string_fill_random(keys[i], 10);
    keys[i][10] = 0;
    hashmap_set(hashmap, keys[i], &values[i]);
  }

  uint size = 0;
  foreach(HashMapValueContainer, hashmap, iter) size++;

  if (hashmap_size(hashmap) != size)
    return test_failed("test_hashmap_size", "Size was incorrect after adding values", __FILE__, __LINE__);

  for (uint i = value_count / 2; i < value_count; ++i) {
    values[i] = rand() % 0x11111111;
    string_fill_random(keys[i], 10);
    keys[i][10] = 0;
    hashmap_set(hashmap, keys[i], &values[i]);
  }

  size = 0;
  foreach(HashMapValueContainer, hashmap, iter) size++;

  if (hashmap_size(hashmap) != size)
    return test_failed("test_hashmap_size", "Size was incorrect after adding values", __FILE__, __LINE__);

  hashmap_clear(hashmap);

  size = 0;
  foreach(HashMapValueContainer, hashmap, iter) size++;

  if (hashmap_size(hashmap) != size)
    return test_failed("test_hashmap_size", "Size was incorrect after clearing values", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

static uint test_hashmap_clear(void) {
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

  hashmap_clear(hashmap);

  if (hashmap_size(hashmap) != 0)
    return test_failed("test_hashmap_clear", "Hashmap was not empty after clearing", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

static uint test_hashmap_write_to_file(void) {
  HashMap *hashmap_read  = hashmap_create();
  HashMap *hashmap_write = hashmap_create();

  uint value_count = 20;
  char keys[20][11];
  int values[20];
  for range(i, 0, value_count - 1) {
    values[i] = rand() % 0x11111111;
    string_fill_random(keys[i], 10);
    keys[i][10] = 0;
    hashmap_set(hashmap_write, keys[i], &values[i]);
  }

  FILE *file = fopen("hashmap_write_test", "wb");
  hashmap_write_to_file(hashmap_write, sizeof(int), file);
  fclose(file);

  file = fopen("hashmap_write_test", "rb");
  hashmap_read_from_file(hashmap_read, sizeof(int), file);
  fclose(file);

  for range(i, 0, value_count - 1) {
    int *value_read = hashmap_get(hashmap_read, keys[i]);
    int *value_write = hashmap_get(hashmap_write, keys[i]);

    if (value_write == NULL)
      return test_failed("test_hashmap_write_to_file", "Value was null for key written to file", __FILE__, __LINE__);

    if (value_read == NULL)
      return test_failed("test_hashmap_write_to_file", "Value was null for key read from file", __FILE__, __LINE__);

    if (*value_read != *value_write)
      return test_failed("test_hashmap_write_to_file", "Key-value pair incorrect when writing to file and reading back again", __FILE__, __LINE__);
  }

  hashmap_destroy(hashmap_read);
  hashmap_destroy(hashmap_write);
  return SUCCESS;
}

uint test_hashmap(void) {
  printf("\nTesting hashmap...\n");

  CHECK_TEST(test_hashmap_create());
  CHECK_TEST(test_hashmap_set());
  CHECK_TEST(test_hashmap_size());
  CHECK_TEST(test_hashmap_clear());
  CHECK_TEST(test_hashmap_write_to_file());

  printf("All hashmap tests passed!\n");
  return SUCCESS;
}
