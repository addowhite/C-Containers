#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "px_string.h"
#include "iter.h"

struct PrivateString {
  IterType type;
  uint length;
  char buffer[];
};

String * string_create(char *value) {
  uint length = strlen(value);
  String *new_str = malloc(sizeof(*new_str) + length + 1);
  if (!new_str) {
    fprintf(stderr, "%s: Failed to allocate memory for string\n", __FILE__);
    return NULL;
  }
  new_str->type = ITER_TYPE_STRING;
  new_str->length = length;
  memcpy(new_str->buffer, value, length);
  new_str->buffer[length] = 0;
  return new_str;
}

char *string_c_str(String *str) {
  return str->buffer;
}

uint string_length(String *str) {
  if (!str) return 0;
  return str->length;
}

char string_get(String *str, uint index) {
  if (index > str->length - 1) {
    fprintf(stderr, "%s: String index out of bounds", __FILE__);
    return ERROR;
  }
  return str->buffer[index];
}

uint string_set(String *str, uint index, char new_chr) {
  if (index > str->length - 1) {
    fprintf(stderr, "%s: String index out of bounds", __FILE__);
    return ERROR;
  }
  str->buffer[index] = new_chr;
  return NO_ERROR;
}

String * string_concat(String *a, String *b) {
  if (!a && !b)
    return NULL;

  uint a_len = string_length(a), b_len = string_length(b);
  char *new_str = malloc(a_len + b_len + 1);

  if (!a) {
    memcpy(new_str, b->buffer, b_len + 1);
  } else if (!b) {
    memcpy(new_str, a->buffer, a_len + 1);
  } else {
    if (!new_str) {
      fprintf(stderr, "%s: Failed to allocate memory for string", __FILE__);
      return NULL;
    }
    memcpy(new_str, a->buffer, a_len);
    memcpy(new_str + a_len, b->buffer, b_len);
    new_str[a_len + b_len] = 0;
  }

  return string_create(new_str);
}

String * string_concat_multi(uint str_count, ...) {
  va_list arg_list;
  va_start(arg_list, str_count);

  String *temp_str, *final_str = NULL;

  for (uint i = 0; i < str_count; ++i) {
    temp_str = string_concat(final_str, va_arg(arg_list, String *));
    if (final_str)
      string_destroy(final_str);
    final_str = temp_str;
  }

  va_end(arg_list);
  return final_str;
}

int string_find_char(String *str, char target) {
  int index = -1;

  for (char *iter = str->buffer, *end = iter + string_length(str); iter != end; ++iter) {
    index++;
    if (*iter == target)
      return index;
  }

  return -1;
}

int string_find_last_char(String *str, char target) {
  int index = string_length(str);

  for (char *end = str->buffer - 1, *iter = end + index; iter != end; --iter) {
    index--;
    if (*iter == target)
      return index;
  }

  return -1;
}

int string_find_string(String *haystack, String *needle) {
  int substr_index = -1;
  uint needle_length = string_length(needle);

  for (char *haystack_iter = haystack->buffer, *haystack_end = haystack_iter + string_length(haystack); haystack_iter != haystack_end; ++haystack_iter) {
    substr_index++;
    bool full_match = true;
    for (char *substr_iter = haystack_iter, *needle_iter = needle->buffer, *needle_end = needle_iter + needle_length; substr_iter != haystack_end && needle_iter != needle_end; ++substr_iter, ++needle_iter) {
      if (*substr_iter != *needle_iter) {
        full_match = false;
        break;
      }
    }

    if (full_match)
      return substr_index;
  }

  return -1;
}

void string_destroy(String *str) {
  free(str->buffer);
  free(str);
}
