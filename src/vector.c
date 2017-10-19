#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "vector.h"
#include "iter.h"
#include "utils.h"

struct PrivateVector {
  IterType type;
  Vector *linearized;
  uchar **data;
  uint size;
  uint capacity;
};

//region static functions
  static uint vector_change_capacity(Vector *vector, uint new_capacity) {
    if (new_capacity == 0) {
      free(vector->data);
      vector->data = NULL;
      vector->capacity = new_capacity;
      vector->size = min(vector->size, new_capacity);
      return NO_ERROR;
    }

    uchar **new_block = realloc(vector->data, sizeof(*vector->data) * new_capacity);
    if (!new_block) {
      fprintf(stderr, "%s: Failed to resize vector\n", __FILE__);
      return ERROR;
    }
    vector->data = new_block;
    if (new_capacity > vector->capacity)
      memset(vector->data + vector->capacity, 0, sizeof(*vector->data) * (new_capacity - vector->capacity));
    vector->capacity = new_capacity;
    return NO_ERROR;
  }
//endregion

Vector * vector_create() {
  Vector *new_vector = malloc(sizeof(*new_vector));
  if (!new_vector) {
    fprintf(stderr, "%s: Failed to create vector\n", __FILE__);
    return NULL;
  }
  new_vector->type = ITER_TYPE_VECTOR;
  new_vector->size = 0;
  new_vector->capacity = 0;
  new_vector->data = NULL;
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
    uint new_capacity = max(VECTOR_MIN_CAPACITY, vector->capacity);
    while ((new_capacity = new_capacity * 2) < new_size);
    if (!vector_change_capacity(vector, new_capacity)) {
      fprintf(stderr, "%s: Failed to change vector capacity during resize operation\n", __FILE__);
      return ERROR;
    }
  }

  if (new_size < vector->size)
    memset(vector->data + new_size, 0, sizeof(*vector->data) * (vector->size - new_size));

  vector->size = new_size;
  return NO_ERROR;
}

uint vector_push_back(Vector *vector, void *value) {
  if (vector->size >= vector->capacity) {
    if (!vector_change_capacity(vector, max(VECTOR_MIN_CAPACITY, vector->capacity * 2))) {
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
  if (new_size > dst_vector->capacity) {
    uint new_capacity = max(VECTOR_MIN_CAPACITY, dst_vector->capacity);
    while ((new_capacity = new_capacity * 2) < new_size);
    if (!vector_change_capacity(dst_vector, new_capacity)) {
      fprintf(stderr, "%s: Failed to change vector capacity when pushing vector to vector\n", __FILE__);
      return ERROR;
    }
  }
  memcpy(dst_vector->data + dst_vector->size, src_vector->data, sizeof(*src_vector->data) * src_vector->size);
  dst_vector->size = new_size;
  return NO_ERROR;
}

uint vector_clear(Vector *vector) {
  return vector_resize(vector, 0);
}

uint vector_copy(Vector *src_vector, int src_index, Vector *dst_vector, int dst_index, uint length) {
  if (src_index < 0)
    fprintf(stderr, "%s: Src index was negative when copying vector\n", __FILE__);

  if (dst_index + length > vector_size(dst_vector))
    fprintf(stderr, "%s: Vector copy past end of destination\n", __FILE__);

  if (dst_index < 0)
    fprintf(stderr, "%s: Src index was negative when copying vector\n", __FILE__);

  if (src_index + length > vector_size(src_vector))
    fprintf(stderr, "%s: Vector copy past end of source\n", __FILE__);

  memmove(dst_vector->data + dst_index, src_vector->data + src_index, sizeof(*src_vector->data) * length);
  return NO_ERROR;
}

uint vector_move(Vector *src_vector, int src_index, Vector *dst_vector, int dst_index, uint length) {
  uint full_length = length;

  if (src_index < 0) {
    dst_index -= src_index;
    length += src_index;
    src_index = 0;
  }

  if (dst_index < 0) {
    src_index -= dst_index;
    length += dst_index;
    dst_index = 0;
  }
  uint src_safe_length = min(length, vector_size(src_vector) - src_index);
  uint dst_safe_length = min(length, vector_size(dst_vector) - dst_index);
  uint safe_length = min(src_safe_length, dst_safe_length);

  if (!vector_copy(src_vector, src_index, dst_vector, dst_index, safe_length)) {
    fprintf(stderr, "%s: Failed to copy vector during move operation\n", __FILE__);
    return ERROR;
  }

  int erase_index = src_index, erase_count = src_safe_length;
  if (src_vector == dst_vector && (
    (dst_index >= src_index && dst_index < src_index + full_length) ||
    (src_index >= dst_index && src_index < dst_index + full_length) ||
    src_index >= vector_size(src_vector)
  )) {
    int diff = dst_index - src_index;
    erase_count = abs(diff);
    erase_index = src_index + ((diff < 0) ? src_safe_length - erase_count : 0);
  }
  vector_erase(src_vector, erase_index, erase_count);
  return NO_ERROR;
}

uint vector_erase(Vector *vector, uint index, uint length) {
  if (index + length > vector_size(vector))
    fprintf(stderr, "%s: Erasing past end of vector\n", __FILE__);
  memset(vector->data + index, 0, sizeof(*vector->data) * length);
  return NO_ERROR;
}

uint vector_read_from_file(Vector *vector, uint element_size, FILE *file) {
  uint vector_length;
  fread(&vector_length, sizeof(vector_length), 1, file);
  vector_resize(vector, vector_length);

  unsigned char *values = malloc(element_size * vector_length);
  fread(values, element_size * vector_length, 1, file);

  unsigned char *val;
  for range(i, 0, vector_length - 1) {
    val = malloc(element_size);
    memcpy(val, values + element_size * i, element_size);
    vector_set(vector, i, val);
  }

  free(values);
  return NO_ERROR;
}

uint vector_write_to_file(Vector *vector, uint element_size, FILE *file) {
  uint vector_length = vector_size(vector);
  fwrite(&vector_length, sizeof(vector_length), 1, file);

  unsigned char *values = malloc(element_size * vector_length);
  if (!values) {
    fprintf(stderr, "%s: Failed to create buffer to write to file\n", __FILE__);
    return ERROR;
  }

  for range(i, 0, vector_length - 1) {
    unsigned char *chr = vector_get(vector, i);
    if (chr == NULL) {
      for range(j, 0, element_size - 1)
        values[i * element_size + j] = 0;
    } else {
      for range(j, 0, element_size - 1)
        values[i * element_size + j] = *(chr + j);
    }
  }

  fwrite(values, element_size * vector_length, 1, file);
  free(values);
  return NO_ERROR;
}

void vector_destroy(Vector *vector) {
  free(vector->data);
  vector->data = NULL;
  free(vector);
  vector = NULL;
}
