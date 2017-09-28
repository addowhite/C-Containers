#ifndef _ITER_H_
#define _ITER_H_

#include "Vector.h"
#include "Grid.h"
#include "HashMap.h"

#ifndef NULL
#define NULL (void *)0
#endif

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

// /*
#define foreach(type, iterable, iter) for (Iterable *vector = (Iterable *)linearize_iterable((Iterable *)(iterable)), *flag = NULL; flag == NULL; flag = (void *)1)\
                                         for (char *FOREACH_PTR_##iter = (char *)vector->data, *end = FOREACH_PTR_##iter + sizeof(type**) * vector->size; FOREACH_PTR_##iter != end; FOREACH_PTR_##iter += sizeof(type**))\
                                            for (type* iter = *(type**)FOREACH_PTR_##iter, *flag2 = NULL; flag2 == NULL; flag2 = (void *)1)
// */
/*
#define foreach(type, iterable, iter)   for (Vector *vector = linearize_iterable((Iterable *)(iterable)), *flag = NULL; flag == NULL; flag = (void *)1)\
                                            for (uint FOREACH_INDEX_##iter = 0, size = vector_size(vector); FOREACH_INDEX_##iter < size; ++FOREACH_INDEX_##iter)\
                                                for (type* iter = (type*)vector_get(vector, FOREACH_INDEX_##iter), *flag2 = NULL; flag2 == NULL; flag2 = (void *)1)
*/
typedef enum IterType {
    ITER_TYPE_VECTOR,
    ITER_TYPE_GRID,
    ITER_TYPE_HASHMAP
} IterType;

typedef struct Iterable {
    IterType type;
    uchar **data;
    uint size;
} Iterable;

Vector * linearize_iterable(Iterable *iterable);

#endif // _ITER_H_
