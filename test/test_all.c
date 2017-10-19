#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "test_vector.h"
#include "test_grid.h"
#include "test_hashmap.h"
#include "test_iter.h"
#include "test_string.h"

int main(int argc, char **argv) {
  // Redirect all errors from stderr to the file "errors.txt"
  freopen("errors.txt", "w+", stderr);

  // Set random seed
	time_t t;
	srand((uint) time(&t));

  test_iter();
  test_vector();
  test_grid();
  test_hashmap();
  test_string();
  return 0;
}
