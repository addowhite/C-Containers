#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#include "vector.h"
#include "iter.h"
#include "utils.h"

struct PrivateHashMap {
  IterType type;
  Vector *linearized;
  Vector *bins[HASHMAP_BIN_COUNT];
};

//region static functions
  /*
   * Jenkins's one-at-a-time hash function
   * https://en.wikipedia.org/wiki/Jenkins_hash_function
   */
  static uint hashmap_get_hash(const char *key) {
    uint hash = 0;
    for (char *currentChr = (char *)key; *currentChr != 0; ++currentChr) {
      hash += *currentChr;
      hash += hash << 10;
      hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash % HASHMAP_BIN_COUNT;
  }
//endregion

HashMap * hashmap_create() {
  HashMap *new_hashmap = malloc(sizeof(*new_hashmap));
  if (!new_hashmap) {
    fprintf(stderr, "%s: Failed to create hashmap\n", __FILE__);
    return NULL;
  }
  new_hashmap->type = ITER_TYPE_HASHMAP;
  new_hashmap->linearized = vector_create();
  if (!new_hashmap->linearized) {
    fprintf(stderr, "%s: Failed to create vector for hashmap\n", __FILE__);
    free(new_hashmap);
    return NULL;
  }
  for (uint i = 0; i < HASHMAP_BIN_COUNT; ++i)
    new_hashmap->bins[i] = NULL;
  return new_hashmap;
}

uint hashmap_set(HashMap *hashmap, const char *key, void *value) {

  //region Get vector for the bin that corresponds to the key (create it if it doesn't exist)
    uint hash = hashmap_get_hash(key);
    Vector *vector = hashmap->bins[hash];
    if (vector == NULL) {
      vector = vector_create();
      if (!vector) {
        fprintf(stderr, "%s: Failed to create vector for hashmap bin\n", __FILE__);
        return ERROR;
      }
      hashmap->bins[hash] = vector;
    }
  //endregion

  //region Create the value container and populate it with the key and value
    HashMapValueContainer *value_container = malloc(sizeof(*value_container));
    if (!value_container) {
      fprintf(stderr, "%s: Failed to create ValueContainer for hashmap\n", __FILE__);
      return ERROR;
    }
    value_container->key = utils_strdup(key);
    value_container->value = value;
  //endregion

  if (!vector_push_back(vector, value_container)) {
    fprintf(stderr, "%s: Failed to push value to vector when adding value to hashmap\n", __FILE__);
    return ERROR;
  }
  return NO_ERROR;
}

void * hashmap_get(HashMap *hashmap, const char *key) {
  Vector *vector = hashmap->bins[hashmap_get_hash(key)];
  if (vector == NULL) return NULL;

  HashMapValueContainer *value_container;
  for (uint i = 0; i < vector_size(vector); ++i) {
    value_container = vector_get(vector, i);
    if (strcmp(value_container->key, key) == 0)
      return value_container->value;
  }

  return NULL;
}

uint hashmap_size(HashMap *hashmap) {
  uint size = 0;
  for (Vector **iter = hashmap->bins, **end = iter + HASHMAP_BIN_COUNT; iter != end; ++iter)
    if (*iter) size += vector_size(*iter);
  return size;
}

uint hashmap_read_from_file(HashMap *hashmap, uint element_size, FILE *file) {
  uint key_length;
  char *key, *value;
  while (!feof(file)) {
    if (fread(&key_length, sizeof(key_length), 1, file) != 1) {
      if (feof(file)) break;
      fprintf(stderr, "%s: Failed to read hashmap key length from file\n", __FILE__);
      return ERROR;
    }

    key = malloc(key_length + 1);
    if (!key) {
      fprintf(stderr, "%s: Failed to allocate memory for hashmap key\n", __FILE__);
      return ERROR;
    }
    if (fgets(key, key_length + 1, file) == NULL) {
      fprintf(stderr, "%s: Error reading hashmap key from file\n", __FILE__);
      free(key);
      return ERROR;
    }

    value = malloc(element_size);
    if (!value) {
      fprintf(stderr, "%s: Failed to allocate memory for hashmap value\n", __FILE__);
      free(key);
      return ERROR;
    }
    if (fread(value, element_size, 1, file) != 1) {
      if (feof(file)) {
        fprintf(stderr, "%s: Unexpected EOF encountered\n", __FILE__);
      } else {
        fprintf(stderr, "%s: Error reading hashmap value from file\n", __FILE__);
      }
      free(key);
      free(value);
      return ERROR;
    }

    if (!hashmap_set(hashmap, key, value)) {
      fprintf(stderr, "%s: Failed to set hashmap value\n", __FILE__);
      free(key);
      free(value);
      return ERROR;
    }
  }
  return NO_ERROR;
}

uint hashmap_write_to_file(HashMap *hashmap, uint element_size, FILE *file) {
  uint key_length;
  foreach(HashMapValueContainer, hashmap, value_container) {
    key_length = strlen(value_container->key);
    fwrite(&key_length, sizeof(key_length), 1, file);
    fputs(value_container->key, file);
    fwrite(value_container->value, element_size, 1, file);
  }
  return NO_ERROR;
}

uint hashmap_clear(HashMap *hashmap) {
  vector_clear(hashmap->linearized);
  for (Vector **iter = hashmap->bins, **end = iter + HASHMAP_BIN_COUNT; iter != end; ++iter) {
    if (*iter) {
      foreach(HashMapValueContainer, *iter, value_container) {
        free((char *)value_container->key);
        free(value_container);
      }
      vector_clear(*iter);
    }
  }
  return NO_ERROR;
}

void hashmap_destroy(HashMap *hashmap) {
  vector_destroy(hashmap->linearized);

  for (Vector **iter = hashmap->bins, **end = iter + HASHMAP_BIN_COUNT; iter != end; ++iter) {
    if (*iter) {
      foreach(HashMapValueContainer, *iter, value_container) {
        free((char *)value_container->key);
        free(value_container);
      }
      vector_destroy(*iter);
    }
  }
  free(hashmap);
}
