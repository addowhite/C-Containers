#ifndef _ITER_H_
#define _ITER_H_

#include "vector.h"
#include "grid.h"
#include "hashmap.h"

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
// #define foreach(type, iterable, iter) for (Iterable *vector = (Iterable *)linearize_iterable((Iterable *)(iterable)), *flag = NULL; flag == NULL; flag = (void *)1)\
//                                          for (uchar **FOREACH_PTR_##iter = vector->data, **end = FOREACH_PTR_##iter + sizeof(uchar **) * vector->size; FOREACH_PTR_##iter != end; FOREACH_PTR_##iter += sizeof(uchar **))
//                                          //    for (type* iter = *(type**)FOREACH_PTR_##iter, *flag2 = NULL; flag2 == NULL; flag2 = (void *)1)
// */
/*
#define foreach(type, iterable, iter)   for (Vector *vector = linearize_iterable((Iterable *)(iterable)), *flag = NULL; flag == NULL; flag = (void *)1)
                                            //for (uint FOREACH_INDEX_##iter = 0, size = vector_size(vector); FOREACH_INDEX_##iter < size; ++FOREACH_INDEX_##iter)
                                                // for (type* iter = (type*)vector_get(vector, FOREACH_INDEX_##iter), *flag2 = NULL; flag2 == NULL; flag2 = (void *)1)
*/

/*
#define foreach(type, container, iter)   for (Iterable *iterable = (Iterable *)linearize_iterable((Iterable *)(container)), *flag = NULL; flag == NULL; flag = (void *)1)\
                                            for (uint FOREACH_INDEX_##iter = 0, FOREACH_SIZE_##iter = iterable->size; FOREACH_INDEX_##iter < FOREACH_SIZE_##iter; ++FOREACH_INDEX_##iter)\
                                                for (type* iter = (type*)iterable->data[FOREACH_INDEX_##iter], *flag2 = NULL; flag2 == NULL; flag2 = (void *)1)
*/
///*
#define foreach(type, container, iter)   for (Iterable *iterable = (Iterable *)linearize_iterable((Iterable *)(container)), *FOREACH_FLAG_##iter = NULL; FOREACH_FLAG_##iter == NULL; FOREACH_FLAG_##iter = (void *)1)\
                                            for (uchar **FOREACH_PTR_##iter = iterable->data, **FOREACH_END_##iter = iterable->data + iterable->size; FOREACH_PTR_##iter != FOREACH_END_##iter; ++FOREACH_PTR_##iter)\
                                                for (type* iter = (type*)(*FOREACH_PTR_##iter), *FOREACH_FLAG2_##iter = NULL; FOREACH_FLAG2_##iter == NULL; FOREACH_FLAG2_##iter = (void *)1)
//*/

#define range(index, start, end) (int index = start, END_##index = range_end(index, end); index != END_##index; index = range_next(index, END_##index))

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

static int range_next(int index, int end) { return (index < end) ? ++index : --index; };
static int range_end(int start, int end) { return (start < end) ? ++end : --start; };
Vector * linearize_iterable(Iterable *iterable);

#endif // _ITER_H_
