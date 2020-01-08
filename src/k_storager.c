#include <uv.h>
#include <stdlib.h>
#include "server.h"

int main (int argc, char **argv) {
  return server_listen(uv_default_loop(), 7000);
}