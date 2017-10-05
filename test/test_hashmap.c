#include <stdlib.h>
#include <stdio.h>
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

  foreach(HashMapValueContainer, hashmap, iter)
    if (hashmap_get(hashmap, iter->key) != iter->value)
      return test_failed("test_iter_hashmap", "Key-value pair incorrect", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

uint test_hashmap(void) {
  printf("\nTesting hashmap...\n");

  CHECK_TEST(test_hashmap_create());
  CHECK_TEST(test_hashmap_set());

  printf("All hashmap tests passed!\n");
  return SUCCESS;
}
