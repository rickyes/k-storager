#include "store.h"

void store_init() {
  store = hashmap_new();
}

int store_put(char* key, char* value) {
  printf("set %s %s\n", key, value);
  return hashmap_put(store, key, value);
}

int store_get(char* key, char args[]) {
  return hashmap_get(store, key, (void**)(&args));
}

int store_remove(char* key) {
  return hashmap_remove(store, key);
}

int store_length() {
  return hashmap_length(store);
}