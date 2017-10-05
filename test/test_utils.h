#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#ifndef uint
#define uint unsigned int
#endif // uint

#ifndef SUCCESS
#define SUCCESS 1
#endif // SUCCESS

#ifndef FAILURE
#define FAILURE 0
#endif // FAILURE

#define CHECK_TEST(func) if (!func) return FAILURE;

uint test_failed(const char *test_name, const char *failure_reason, const char *file_name, uint line_number);
char * string_fill_random(char *str, uint count);

#endif // _TEST_UTILS_H_
