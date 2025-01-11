#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <signal.h>

#include "server_handler.h"
#include "../libconnection/socket/socket_server.h"
#include "../libstructures/syn_buffer.h"
#include "../libshared/game.h"
#include "../libshared/command_types.h"

#define LIMIT_X 50
#define LIMIT_Y 30
#define LIMIT_R 6
#define CIRCLE_MICROS 1600000

typedef struct server_event {
    int type;
    void* data;
} server_event;

#define SERVER_EVENT_RECV 0
#define SERVER_EVENT_CIRCLE 1
#define SERVER_TIMER_TICK 3

typedef struct server_recv_data {
    int con_id;
    void* data;
    size_t len;
} server_recv_data;

void on_recv(int con_id, void* data, size_t len, void* context) {
    if (len == 0) {
        printf("%d disconnect!\n", con_id);
        return;
    }
    server_recv_data* recv_data = malloc(sizeof(server_recv_data));
    recv_data->con_id = con_id;
    recv_data->data = data;
    recv_data->len = len;
    server_event event = (server_event) {SERVER_EVENT_RECV, data};
    syn_buffer_add(context, &event);
}

void* game_loop(void* arg) {
    syn_buffer* buff = arg;
    int circle_id = 0;
    for (;;) {
        command_circle_data* circle = malloc(sizeof(command_circle_data));
        *circle = (command_circle_data){
            circle_id++, rand()%LIMIT_X, rand()%LIMIT_Y, rand()%LIMIT_R, rand()%8
        };
        server_event event = (server_event) {SERVER_EVENT_CIRCLE, &circle};
        syn_buffer_add(buff, &event);
        usleep(rand()%CIRCLE_MICROS);
    }
    return NULL;
}

void* timer_tick(void* arg) {
    syn_buffer* buff = arg;
    for (;;) {
        server_event event = (server_event) {SERVER_TIMER_TICK, NULL};
        syn_buffer_add(buff, &event);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char** argv) {
    server s;
    syn_buffer event_buffer;

    _Bool running = 0;

    // TODO: handle sigint

    int res = socket_server_start(&s, 28565, on_recv, &event_buffer);
    if (res > 0) {
        fprintf(stderr, "%s\n", strerror(errno));
    }

    game game;
    game_init(&game);
    pthread_t game_thread;

    syn_buffer_init(&event_buffer, 16, sizeof(server_event));
    while (!running) {
        server_event message;
        syn_buffer_get(&event_buffer, &message);
        if (message.type == SERVER_EVENT_RECV) {
            server_recv_data* recv_data = message.data;
            handle_command(recv_data->con_id, recv_data->data, recv_data->len, &event_buffer);
        }
        if (message.type == SERVER_EVENT_CIRCLE) {

        }
        if (message.type == SERVER_TIMER_TICK) {
            if (!--game.time) {
                //game ended!!
            }
        }
    }
    game_free(&game);
}