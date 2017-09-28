#include <stdio.h>
#include "iter.h"

Vector * linearize_iterable(Iterable *iterable) {
    switch (iterable->type) {
        case ITER_TYPE_VECTOR:
            return (Vector *)iterable;
            break;

        case ITER_TYPE_GRID:
            return *(Vector **)((char *)iterable + sizeof(IterType));
            break;

        case ITER_TYPE_HASHMAP:
            {
                Vector *linearized_vector = *(Vector **)((char *)iterable + sizeof(IterType));
                Vector **bins = (Vector **)((char *)iterable + sizeof(IterType) + sizeof(Vector *));
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
