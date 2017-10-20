#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

int min(int a, int b) {
	return (a < b) ? a : b;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

char *utils_strdup(const char *str) {
  uint length = strlen(str) + 1;
  char *new_str = malloc(length);
  if (!new_str) {
    fprintf(stderr, "%s[%d]: Failed to duplicate string", __FILE__, __LINE__);
    return NULL;
  }
  memcpy(new_str, str, length);
  return new_str;
}
