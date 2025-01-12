#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H

#include "../libshared/game.h"
#include "../libconnection/server.h"

typedef struct server_context {
    server server;
    game* game;
    int client_count;
} server_context;

#endif
