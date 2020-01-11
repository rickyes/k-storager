#include "store.h"
#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cJSON.h"

uv_loop_t *serverLoop;

void free_write_req(uv_write_t *req) {
    write_req_t *wr = (write_req_t*) req;
    free(wr->buf.base);
    free(wr);
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*) malloc(suggested_size);
    buf->len = suggested_size;
}

void on_close(uv_handle_t* handle) {
    free(handle);
}

void echo_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
    }
    free_write_req(req);
}

void echo_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        uv_write_t* w_req = (uv_write_t*)malloc(sizeof(uv_write_t));
        cJSON *json = cJSON_Parse(buf->base);
        printf("Hi %s\n", cJSON_Print(json));
        cJSON *type = cJSON_GetObjectItemCaseSensitive(json, "type");
        cJSON *key = cJSON_GetObjectItemCaseSensitive(json, "key");
        cJSON *value = cJSON_GetObjectItemCaseSensitive(json, "value");
        if (cJSON_IsString(type) == FALSE || (type->valuestring == NULL)) {
            fprintf(stderr, "check type failed %s\n", type->valuestring);
        }
        // if (cJSON_IsString(key) == FALSE || (key->valuestring == NULL)) {
        //     fprintf(stderr, "check key failed %s\n", key->valuestring);
        // }
        // if (cJSON_IsString(value) == FALSE || (value->valuestring == NULL)) {
        //     fprintf(stderr, "check value failed %s\n", value->valuestring);
        // }
        if (strcmp(type->valuestring, "set") == 0) {
            int error = store_put(key->valuestring, value->valuestring);
            char status[10] = "fail";
            if (error == 0) {
                strcpy(status, "success");
            }
            uv_buf_t write_buf = uv_buf_init(status, strlen(status));
            uv_write((uv_write_t*) w_req, client, &write_buf, 1, echo_write);
            return;
        } else if (strcmp(type->valuestring, "get") == 0) {
            char *resp_body = "";
            int error = hashmap_get(store, key->valuestring, (void**)(&resp_body));
            printf("get data: %s\n", resp_body);
            uv_buf_t write_buf = uv_buf_init(resp_body, strlen(resp_body));
            uv_write((uv_write_t*) w_req, client, &write_buf, 1, echo_write);
            return;
        }
        uv_buf_t write_buf = uv_buf_init(buf->base, nread);
        uv_write((uv_write_t*) w_req, client, &write_buf, 1, echo_write);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) client, on_close);
    }

    free(buf->base);
}

void connection(uv_stream_t *server, int status) {
    fprintf(stdout, "New connection... status: %d\n", status);
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }

    uv_tcp_t *client = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(serverLoop, client);
    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        uv_read_start((uv_stream_t*) client, alloc_buffer, echo_read);
    }
    else {
        uv_close((uv_handle_t*) client, on_close);
    }
}

int server_listen(uv_loop_t *loop, int port) {

  serverLoop = loop;
  
  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  struct sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", port, &addr);
  uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);

  int status = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, connection);
  if (status) {
    fprintf(stderr, "Listen error %s\n", uv_strerror(status));
    return 1;
  }
  store_init();
  fprintf(stdout, "Start TCP Server for port: %d\n", port);
  return uv_run(loop, UV_RUN_DEFAULT);
}