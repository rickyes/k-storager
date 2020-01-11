#ifndef SRC_STORE_H_
#define SRC_STORE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

map_t store;

void store_init();
int store_put(char* key, char* value);
int store_get(char* key, char args[]);
int store_remove(char* key);
int store_length();


#endif /* SRC_STORE_H_ */

