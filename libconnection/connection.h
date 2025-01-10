#ifndef CONNECTION_H
#define CONNECTION_H

#include <pthread.h>

typedef struct connection {
    void* connection_data;
    void(*send)(struct connection*, void*, size_t size);
    void(*on_receive)(void*, size_t, void*);
    void* context;
    void(*close)(struct connection*);
    int id;
} connection;

void connection_close(connection* con);
void connection_send(connection* con, void* data, size_t size);

#endif
