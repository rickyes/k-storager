#ifndef K_STORAGER_SRC_COMMAND_H_
#define K_STORAGER_SRC_COMMAND_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <hashmap.h>

typedef struct {
  char* command;
  char *value;
} k_storager_req;

#endif /* K_STORAGER_SRC_COMMAND_H_ */