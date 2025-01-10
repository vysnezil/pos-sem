#ifndef SOCKET_SERVER_H
#define SOCKET_SERVER_H

#include "../server.h"

int socket_server_start(server* this, int port, void(*on_receive)(int, void*, size_t, void*), void* context);

#endif
