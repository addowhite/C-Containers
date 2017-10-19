#ifndef _ITER_H_
#define _ITER_H_

#include "utils.h"
#include "vector.h"

#define foreach(type, container, iter)  for (Iterable *iterable = (Iterable *)linearize_iterable((Iterable *)(container)), *FOREACH_FLAG_##iter = NULL; FOREACH_FLAG_##iter == NULL; FOREACH_FLAG_##iter = (void *)1)\
                                          for (uchar **FOREACH_PTR_##iter = iterable->data, **FOREACH_END_##iter = iterable->data + iterable->size; FOREACH_PTR_##iter != FOREACH_END_##iter; ++FOREACH_PTR_##iter)\
                                            for (type* iter = (type*)(*FOREACH_PTR_##iter), *FOREACH_FLAG2_##iter = NULL; FOREACH_FLAG2_##iter == NULL; FOREACH_FLAG2_##iter = (void *)1)

#define range(index, start, end) (int index = start, END_##index = iter_range_end(index, end); index != END_##index; index = iter_range_next(index, END_##index))

typedef enum IterType {
  ITER_TYPE_VECTOR,
  ITER_TYPE_GRID,
  ITER_TYPE_HASHMAP,
  ITER_TYPE_STRING
} IterType;

typedef struct Iterable {
  IterType type;
  Vector *linearized;
  uchar **data;
  uint size;
} Iterable;

int iter_range_next(int index, int end);
int iter_range_end(int start, int end);
Vector * linearize_iterable(Iterable *iterable);

#endif // _ITER_H_
