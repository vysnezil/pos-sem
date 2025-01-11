#include "server_handler.h"

#include <stdint.h>
#include <string.h>

#include "../libshared/command_types.h"
#include "../libshared/game.h"

void handle_command(int con_id, void* arg, size_t len, server_context* context) {
    server* s = &context->server;
    game* g = &context->game;
    if (len == 0) {
        if (arg != NULL) {
            command_simple com = (command_simple){ COMMAND_INIT };
            send_data(s, con_id, &com, sizeof(command_simple));
        }
        return;
    }
    if (arg == NULL) {
        // TODO: handle disconnect
        return;
    }
    command_simple* cmd = arg;
    int type =  cmd->type;
    switch (type) {
        case COMMAND_PLAYER: {
            command_player* data = arg;
            player p;
            memcpy(&p.name, data, 30);
            p.id = con_id;
            p.score = 0;
            add_player(g, &p);
            data->player_id = con_id;
            broadcast_data(s, arg, sizeof(command_player));
            break;
        }
    }
}