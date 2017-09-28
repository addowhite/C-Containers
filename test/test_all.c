#include <stdio.h>
#include "test_vector.h"
#include "test_grid.h"
#include "test_hashmap.h"

int main(int argc, char **argv) {
  // Redirect all errors from stderr to the file "errors.txt"
  freopen("errors.txt", "w+", stderr);

  test_vector();
  test_grid();
  test_hashmap();
  return 0;
}