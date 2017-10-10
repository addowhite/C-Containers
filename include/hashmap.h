#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#ifndef uint
#define uint unsigned int
#endif

#ifndef uchar
#define uchar unsigned char
#endif

#define ERROR 0
#define NO_ERROR 1

#define HASHMAP_BIN_COUNT 1024

struct PrivateHashMap;
typedef struct PrivateHashMap HashMap;

typedef struct HashMapValueContainer {
  const char *key;
  void *value;
} HashMapValueContainer;

HashMap * hashmap_create();
uint hashmap_set(HashMap *hashmap, const char *key, void *value);
void * hashmap_get(HashMap *hashmap, const char *key);
uint hashmap_size(HashMap *hashmap);
uint hashmap_clear(HashMap *hashmap);
uint hashmap_read_from_file(HashMap *hashmap, uint element_size, FILE *file);
uint hashmap_write_to_file(HashMap *hashmap, uint element_size, FILE *file);
void hashmap_destroy(HashMap *hashmap);

#endif // _HASHMAP_H_
