#include <stdlib.h>
#include <stdio.h>
#include "test_utils.h"

uint test_failed(const char *test_name, const char *failure_reason, const char *file_name, uint line_number) {
  printf("\nTest failed: \"%s\"\nFile: \"%s\"\nLine: %u.\nReason: %s\n", test_name, file_name, line_number, failure_reason);
  return FAILURE;
}

char * string_fill_random(char *str, uint count) {
  char base = 'A';
  for (uint i = 0; i < count; ++i)
    str[i] = base + rand() % 26;
  return str;
}