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
void vector_clear(Vector *vector);
void vector_destroy();

#endif // _VECTOR_H_
