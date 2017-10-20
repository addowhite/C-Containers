#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "px_string.h"
#include "iter.h"

struct PrivateString {
  IterType type;
  uint length;
  char buffer[];
};

String * string_create_vargs(const char *format, va_list args) {
  int length = 5, required_length = 0;
  char *str = NULL;

  while (1) {
    char *new_buffer = realloc(str, length + 1);
    if (!new_buffer) {
      fprintf(stderr, "%s[%d]: Failed to allocate memory for character array\n", __FILE__, __LINE__);
      free(str);
      return NULL;
    }
    str = new_buffer;

    va_list args_copy;
    va_copy(args_copy, args);
    required_length = vsnprintf(str, length + 1, format, args_copy);
    va_end(args_copy);

    if (length == required_length)
      break;

    length = required_length;
  }

  String *new_string = malloc(sizeof(*new_string) + length + 1);
  if (!new_string) {
    fprintf(stderr, "%s: Failed to allocate memory for string\n", __FILE__);
    return NULL;
  }
  new_string->type = ITER_TYPE_STRING;
  new_string->length = length;
  memcpy(new_string->buffer, str, length);
  new_string->buffer[length] = 0;
  return new_string;
}

String * string_create(const char *format, ...) {
  va_list args;
  va_start(args, format);
  String *new_str = string_create_vargs(format, args);
  va_end(args);
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
