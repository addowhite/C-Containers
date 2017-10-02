#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#include "vector.h"
#include "iter.h"

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
        value_container->key = key;
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

void hashmap_destroy(HashMap *hashmap) {
    vector_clear(hashmap->linearized);
    vector_destroy(hashmap->linearized);
    for (uint i = 0; i < HASHMAP_BIN_COUNT; ++i)
        if (hashmap->bins[i])
            vector_destroy(hashmap->bins[i]);
    free(hashmap);
}
