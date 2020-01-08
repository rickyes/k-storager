#ifndef K_STORAGER_SRC_SERVER_H_
#define K_STORAGER_SRC_SERVER_H_

#include <uv.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_BACKLOG 128

typedef struct {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

void free_write_req(uv_write_t *req);
void on_close(uv_handle_t* handle);
void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
void connection(uv_stream_t *server, int status);
int server_listen(uv_loop_t *loop, int port);

#endif /* K_STORAGER_SRC_SERVER_H_ */

