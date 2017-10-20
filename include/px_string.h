#ifndef _STRING_H_
#define _STRING_H_

#include <stdarg.h>
#include <string.h>
#include "utils.h"

struct PrivateString;
typedef struct PrivateString String;

String * string_create_vargs(const char *format, va_list args);
String * string_create(const char *format, ...);
char *string_c_str(String *str);
uint string_length(String *str);
char string_get(String *str, uint index);
uint string_set(String *str, uint index, char new_chr);
String * string_concat(String *a, String *b);
String * string_concat_multi(uint str_count, ...);
int string_find_char(String *str, char target);
int string_find_last_char(String *str, char target);
int string_find_string(String *haystack, String *needle);
void string_destroy(String *str);

#endif // _STRING_H_
