#ifndef SERVER_H
#define SERVER_H

#include "../libstructures/sll.h"

typedef struct server {
    sll connections;
    void* server_data;
    void(*free_func)(struct server*);
    pthread_t thread;
    void(*on_receive)(int, void*, size_t, void*);
    void* context;
    int last_id;
    pthread_mutex_t mutex;
} server;

void send_data(server* server, int connection_id, void* data, size_t size);
void broadcast_data(server* server, void* data, size_t size);
void server_close_connection(server* server, int connection_id);
void server_destroy(server* server);

#endif
