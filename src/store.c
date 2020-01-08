#include "store.h"

int put(char* key, any_t value) {
  return hashmap_put(store, key, value);
}

int get(char* key, any_t *arg) {
  return hashmap_get(store, key, (void**)(&arg));
}

int remove(char* key) {
  return hashmap_remove(store, key);
}

int length() {
  return hashmap_length(store);
}