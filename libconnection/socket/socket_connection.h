#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H

#include "../connection.h"
#include <stdatomic.h>

typedef struct socket_connection_data {
  int socket;
  pthread_t thread;
  atomic_bool active;
} socket_connection_data;

void socket_connection_init(connection* con, int socket);
int socket_connect(connection* con, char* host, int port, void(*on_receive)(void*, size_t, void*), void* context);

#endif
