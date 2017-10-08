#ifndef _VECTOR_H_
#define _VECTOR_H_

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#define ERROR 0
#define NO_ERROR 1

#include <stdio.h>

#define INITIAL_CAPACITY 1

struct PrivateVector;
typedef struct PrivateVector Vector;

Vector * vector_create();
uint vector_size(Vector *vector);
uint vector_capacity(Vector *vector);
uint vector_resize(Vector *vector, uint new_size);
uint vector_push_back(Vector *vector, void *value);
void * vector_get(Vector *vector, uint index);
uint vector_set(Vector *vector, uint index, void *value);
uint vector_insert(Vector *vector, uint index, void *value);
int vector_find_index(Vector *vector, void *value);
uint vector_push_vector(Vector *dst_vector, Vector *src_vector);
uint vector_clear(Vector *vector);
uint vector_copy(Vector *src_vector, uint src_index, Vector *dst_vector, uint dst_index, uint length);
uint vector_move(Vector *src_vector, uint src_index, Vector *dst_vector, uint dst_index, uint length);
uint vector_erase(Vector *vector, uint index, uint length);
uint vector_read_from_file(Vector *vector, FILE *file, uint element_size);
uint vector_write_to_file(Vector *vector, FILE *file, uint element_size);
void vector_destroy(Vector *vector);

#endif // _VECTOR_H_
