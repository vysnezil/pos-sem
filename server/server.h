#ifndef SERVER_CONTEXT_H
#define SERVER_CONTEXT_H

#include "../libshared/game.h"
#include "../libconnection/server.h"

typedef struct server_context {
    server server;
    game game;
} server_context;

#endif
