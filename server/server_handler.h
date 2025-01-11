#ifndef SERVER_HANDLER_H
#define SERVER_HANDLER_H

#include "server.h"

void handle_command(int con_id, void* arg, size_t len, server_context* context);

#endif