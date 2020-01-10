#ifndef K_STORAGER_SRC_STORE_H_
#define K_STORAGER_SRC_STORE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

map_t store;

int put(char* key, any_t value);
int get(char* key, any_t *arg);
int remove(char* key);
int length();


#endif /* K_STORAGER_SRC_STORE_H_ */

