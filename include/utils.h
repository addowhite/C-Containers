#ifndef _CONTAINER_UTILS_H_
#define _CONTAINER_UTILS_H_

#include <stdint.h>

#define ERROR 0
#define NO_ERROR 1

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#ifndef bool
#define bool uint8_t
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

int min(int a, int b);
int max(int a, int b);
char *utils_strdup(const char *str);

#endif // _CONTAINER_UTILS_H_