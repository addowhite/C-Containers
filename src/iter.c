#include <stdio.h>
#include "iter.h"

Vector * linearize_iterable(Iterable *iterable) {
    switch (iterable->type) {
        case ITER_TYPE_VECTOR:
            return (Vector *)iterable;
            break;

        case ITER_TYPE_GRID:
            return (Vector *)iterable->data;
            break;

        case ITER_TYPE_HASHMAP:
            {
                Vector *linearized_vector = (Vector *)iterable->data;
                Vector **bins = ((Vector **)&iterable->data) + 1;
                vector_clear(linearized_vector);
                for (uint i = 0; i < HASHMAP_BIN_COUNT; ++i)
                    if (bins[i]) vector_push_vector(linearized_vector, bins[i]);
                return linearized_vector;
            }
            break;

        default:
            return NULL;
            break;
    }
}
