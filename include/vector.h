#ifndef _VECTOR_H_
#define _VECTOR_H_

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#include <stdio.h>

#define VECTOR_MIN_CAPACITY 1

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
uint vector_copy(Vector *src_vector, int src_index, Vector *dst_vector, int dst_index, uint length);
uint vector_move(Vector *src_vector, int src_index, Vector *dst_vector, int dst_index, uint length);
uint vector_erase(Vector *vector, uint index, uint length);
uint vector_read_from_file(Vector *vector, uint element_size, FILE *file);
uint vector_write_to_file(Vector *vector, uint element_size, FILE *file);
void vector_destroy(Vector *vector);

#endif // _VECTOR_H_
