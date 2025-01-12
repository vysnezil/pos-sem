#include "server_handler.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../libshared/command_types.h"
#include "../libshared/game.h"

struct forech_sendplayer_context {
    server* srv;
    int con_id;
};

void forech_sendplayer(void* obj_arg, void* context_arg) {
    struct forech_sendplayer_context* cnt = context_arg;
    player* p = obj_arg;
    command_player com = (command_player){ COMMAND_PLAYER, p->id };
    memcpy(com.name, p->name, sizeof(p->name));
    send_data(cnt->srv, cnt->con_id, &com, sizeof(command_player));
}

_Bool id_predicate(void* item, void* data) {
    return ((player*)item)->id == *(int*)data;
}

void handle_command(int con_id, void* arg, size_t len, server_context* context) {
    server* s = &context->server;
    game* g = context->game;
    if (len == 0) {
        if (arg != NULL) {
            context->client_count++;
            command_simple com = (command_simple){ COMMAND_INIT };
            send_data(s, con_id, &com, sizeof(command_simple));
            return;
        }
    }
    if (arg == NULL) {
        context->client_count--;
        remove_player(g, con_id);
        command_player p = (command_player){ COMMAND_PLAYER, con_id, 1};
        broadcast_data(s, &p, sizeof(command_player));
        return;
    }
    command_simple* cmd = arg;
    int type =  cmd->type;
    switch (type) {
        case COMMAND_PLAYER: {
            command_player* data = arg;
            player p;
            memcpy(p.name, data->name, 20);
            p.id = con_id;
            p.score = 0;
            p.tmp_str = 0;
            pthread_mutex_lock(&g->mutex);
            struct forech_sendplayer_context cnt = (struct forech_sendplayer_context) { s, con_id };
            sll_for_each(&g->players, forech_sendplayer, &cnt);
            pthread_mutex_unlock(&g->mutex);
            add_player(g, &p);
            data->player_id = con_id;
            broadcast_data(s, arg, sizeof(command_player));
            break;
        }
        case COMMAND_PLAYER_READY: {
            command_ready* cmd = arg;
            pthread_mutex_lock(&g->mutex);
            if (cmd->ready) g->ready_count++;
            else g->ready_count--;
            pthread_mutex_unlock(&g->mutex);
            break;
        }
        case COMMAND_HIT: {
            command_hit* cmd = arg;
            pthread_mutex_lock(&g->mutex);
            player* p = sll_find(&g->players, id_predicate, &con_id);
            p->score++;
            pthread_mutex_unlock(&g->mutex);
            broadcast_data(s, arg, sizeof(command_hit));
            command_score cmd_s = (command_score) { COMMAND_SCORE, p->score, con_id };
            broadcast_data(s, &cmd_s, sizeof(command_score));
            break;
        }
    }
}