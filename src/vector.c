#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vector.h"
#include "iter.h"

struct PrivateVector {
    IterType type;
    uchar **data;
    uint size;
    uint capacity;
};

//region static functions
    static uint vector_change_capacity(Vector *vector, uint new_capacity) {
        void *new_block = realloc(vector->data, sizeof(*(vector->data)) * new_capacity);
        if (!new_block) {
            fprintf(stderr, "%s: Failed to resize vector\n", __FILE__);
            return ERROR;
        }
        vector->data = (uchar **)new_block;
        memset(vector->data + vector->capacity, 0, sizeof(*(vector->data)) * (new_capacity - vector->capacity));
        vector->capacity = new_capacity;
        return NO_ERROR;
    }
//endregion

Vector * vector_create() {
    Vector *new_vector = malloc(sizeof *new_vector);
    if (!new_vector) {
        fprintf(stderr, "%s: Failed to create vector\n", __FILE__);
        return NULL;
    }
    new_vector->type = ITER_TYPE_VECTOR;
    new_vector->size = 0;
    new_vector->capacity = 0;
    new_vector->data = NULL;
    vector_change_capacity(new_vector, INITIAL_CAPACITY);
    return new_vector;
}

uint vector_size(Vector *vector) {
    return vector->size;
}

uint vector_capacity(Vector *vector) {
    return vector->capacity;
}

uint vector_resize(Vector *vector, uint new_size) {
    if (new_size > vector->capacity) {
        if (!vector_change_capacity(vector, new_size * 1.5f)) {
            fprintf(stderr, "%s: Failed to change vector capacity during resize operation\n", __FILE__);
            return ERROR;
        }
    }
    if (new_size < vector->size) {
        memset(vector->data + new_size, 0, sizeof(*(vector->data)) * (vector->size - new_size));
    }
    vector->size = new_size;
    return NO_ERROR;
}

uint vector_push_back(Vector *vector, void *value) {
    if (vector->size >= vector->capacity) {
        if (!vector_change_capacity(vector, vector->capacity * 2)) {
            fprintf(stderr, "%s: Failed to change vector capacity when pushing value\n", __FILE__);
            return ERROR;
        }
    }
    vector->data[vector->size++] = value;
    return NO_ERROR;
}

void * vector_get(Vector *vector, uint index) {
    if (index >= vector->size) {
        fprintf(stderr, "%s: Vector read out of bounds at index %d\n", __FILE__, index);
        return NULL;
    }
    return vector->data[index];
}

uint vector_set(Vector *vector, uint index, void *value) {
    if (index >= vector->size) {
        fprintf(stderr, "%s: Vector write out of bounds at index %d\n", __FILE__, index);
        return ERROR;
    }
    vector->data[index] = value;
    return NO_ERROR;
}

uint vector_insert(Vector *vector, uint index, void *value) {
    if (index >= vector->size) {
        if (!vector_resize(vector, index + 1)) {
            fprintf(stderr, "%s: Failed to resize vector during insert\n", __FILE__);
            return ERROR;
        }
    } else {
        if (!vector_push_back(vector, NULL)) {
            fprintf(stderr, "%s: Failed to push new value to vector during insert\n", __FILE__);
            return ERROR;
        }
        uchar **start = vector->data + index;
        memmove(start + 1, start, sizeof(*vector->data) * (vector->size - 1 - index));
    }
    vector->data[index] = value;
    return NO_ERROR;
}

int vector_find_index(Vector *vector, void *value) {
    for (uint i = 0; i < vector->size; ++i)
        if (vector->data[i] == (uchar *)value)
            return i;
    return -1;
}

uint vector_push_vector(Vector *dst_vector, Vector *src_vector) {
    uint new_size = dst_vector->size + src_vector->size;
    if (new_size >= dst_vector->capacity) {
        if (!vector_change_capacity(dst_vector, new_size * 1.5f)) {
            fprintf(stderr, "%s: Failed to change vector capacity when pushing vector to vector\n", __FILE__);
            return ERROR;
        }
    }
    memcpy(&dst_vector->data[dst_vector->size], src_vector->data, sizeof(*dst_vector->data) * src_vector->size);
    dst_vector->size = new_size;
    return NO_ERROR;
}

uint vector_clear(Vector *vector) {
    return vector_resize(vector, 0);
}

void vector_destroy(Vector *vector) {
    free(vector->data);
    free(vector);
}
