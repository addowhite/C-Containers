#include <stdio.h>
#include "test_hashmap.h"
#include "hashmap.h"

uint test_hashmap_create(void) {
  HashMap *hashmap = hashmap_create(10, 5);

  if (hashmap == NULL)
    return test_failed("test_hashmap_create", "Created hashmap was NULL", __FILE__, __LINE__);

  hashmap_destroy(hashmap);
  return SUCCESS;
}

uint test_hashmap(void) {
  printf("\nTesting hashmap...\n");

  CHECK_TEST(test_hashmap_create());

  printf("All hashmap tests passed!\n");
  return SUCCESS;
}
