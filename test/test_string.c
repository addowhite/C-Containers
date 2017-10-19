#include <stdio.h>
#include "test_string.h"
#include "px_string.h"
#include "iter.h"

static uint test_string_create(void) {
  String *str = string_create("");

  if (!str)
    return test_failed("test_string", "Failed to create string for test", __FILE__, __LINE__);

  if (string_length(str) != 0)
    return test_failed("test_string", "Size incorrect when creating string", __FILE__, __LINE__);

  string_destroy(str);

  str = string_create("Hello");

  if (!str)
    return test_failed("test_string", "Failed to create string for test", __FILE__, __LINE__);

  if (string_length(str) != 5)
    return test_failed("test_string", "Size incorrect when creating string", __FILE__, __LINE__);

  if (string_get(str, 0) != 'H')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 1) != 'e')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 2) != 'l')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 3) != 'l')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 4) != 'o')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  string_destroy(str);
  return SUCCESS;
}

static uint test_string_set(void) {
  String *str = string_create("#####");

  string_set(str, 0, 'H');
  string_set(str, 1, 'e');
  string_set(str, 2, 'l');
  string_set(str, 3, 'l');
  string_set(str, 4, 'o');

  if (string_get(str, 0) != 'H')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 1) != 'e')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 2) != 'l')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 3) != 'l')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  if (string_get(str, 4) != 'o')
    return test_failed("test_string_set", "Incorrect character returned after set", __FILE__, __LINE__);

  string_destroy(str);
  return SUCCESS;
}

static uint test_string_concat(void) {
  char *data_a = "Hello", *data_b = "World";
  String *string_a = string_create(data_a);
  String *string_b = string_create(data_b);

  String *both = string_concat(string_a, string_b);

  if (strcmp(string_c_str(both), "HelloWorld") != 0)
    return test_failed("test_string_concat", "Value incorrect after string concat", __FILE__, __LINE__);

  string_destroy(string_a);
  string_destroy(string_b);
  return SUCCESS;
}

static uint test_string_concat_multi(void) {
  String *strings[] = {
    string_create("one, "),
    string_create("two, "),
    string_create("three")
  };

  String *all_together_now = string_concat_multi(4, strings[0], strings[1], strings[2], string_create(", four"));
  if (strcmp(string_c_str(all_together_now), "one, two, three, four") != 0)
    return test_failed("test_string_concat_multi", "Value incorrect after string concat", __FILE__, __LINE__);

  return SUCCESS;
}

static uint test_string_find_char(void) {
  String *str = string_create("HelloWorld!");

  if (string_find_char(str, 'H') != 0)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, 'e') != 1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, 'l') != 2)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, 'o') != 4)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, '!') != 10)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, 'G') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, 'h') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_char(str, '&') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  string_destroy(str);
  return SUCCESS;
}

static uint test_string_find_last_char(void) {
  String *str = string_create("HelloWorld!");

  if (string_find_last_char(str, 'H') != 0)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, 'e') != 1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, 'l') != 8)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, 'o') != 6)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, '!') != 10)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, 'G') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, 'h') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  if (string_find_last_char(str, '&') != -1)
    return test_failed("test_string_find_char", "Incorrect index returned", __FILE__, __LINE__);

  string_destroy(str);
  return SUCCESS;
}

static uint test_string_find_string(void) {
  String *str = string_create("HelloWorld!"), *needle;

  needle = string_create("Hello");
  if (string_find_string(str, needle) != 0)
    return test_failed("test_string_find_string", "Incorrect index returned finding substring", __FILE__, __LINE__);
  string_destroy(needle);

  needle = string_create("World");
  if (string_find_string(str, needle) != 5)
    return test_failed("test_string_find_string", "Incorrect index returned finding substring", __FILE__, __LINE__);
  string_destroy(needle);

  return SUCCESS;
}

uint test_string(void) {
  printf("\nTesting string...\n");

  CHECK_TEST(test_string_create());
  CHECK_TEST(test_string_set());
  CHECK_TEST(test_string_concat());
  CHECK_TEST(test_string_concat_multi());
  CHECK_TEST(test_string_find_char());
  CHECK_TEST(test_string_find_last_char());
  CHECK_TEST(test_string_find_string());

  printf("All string tests passed!\n");
  return SUCCESS;
}
