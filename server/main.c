#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <time.h>
#include <signal.h>
#include <stdatomic.h>

#include "server_handler.h"
#include "../libconnection/socket/socket_server.h"
#include "../libstructures/syn_buffer.h"
#include "../libshared/game.h"
#include "../libshared/command_types.h"

#include "server.h"

#define LIMIT_X 60
#define LIMIT_Y 25
#define LIMIT_R 6
#define CIRCLE_MICROS 1600000

typedef struct server_event {
    int type;
    void* data;
} server_event;

#define SERVER_EVENT_RECV 0
#define SERVER_EVENT_CIRCLE 1
#define SERVER_TIMER_TICK 3
#define SERVER_ROUND_DEFAULT 10

#define SERVER_PORT_DEFAULT 28565
#define SERVER_CLOSE_TIME 30

typedef struct server_recv_data {
    int con_id;
    void* data;
    size_t len;
} server_recv_data;

void on_recv(int con_id, void* data, size_t len, void* context) {
    server_recv_data* recv_data = malloc(sizeof(server_recv_data));
    recv_data->con_id = con_id;
    recv_data->data = data;
    recv_data->len = len;
    server_event event = (server_event) {SERVER_EVENT_RECV, recv_data};
    syn_buffer_add(context, &event);
}

void* game_loop(void* arg) {
    syn_buffer* buff = arg;
    int circle_id = 0;
    for (;;) {
        command_circle* circle = malloc(sizeof(command_circle));
        *circle = (command_circle){
            COMMAND_CIRCLE, circle_id++, rand()%LIMIT_X, rand()%LIMIT_Y, rand()%LIMIT_R, (rand()%8)+1
        };
        server_event event = (server_event) {SERVER_EVENT_CIRCLE, circle};
        syn_buffer_add(buff, &event);
        nanosleep((const struct timespec[]){{0, (rand()%CIRCLE_MICROS)*1000}}, NULL);
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

// TODO: fix leaks (if any)

static atomic_bool running = 1;

void cancel(int arg) {
    running = 0;
}

int main(int argc, char** argv) {
    short port = SERVER_PORT_DEFAULT;
    size_t round_time = SERVER_ROUND_DEFAULT;
    _Bool headless = 0;
    if (argc >= 3) {
        port = atoi(argv[1]);
        if (port == 0) {
            fprintf(stderr, "Invalid argument [port]\n");
            return EINVAL;
        }
        round_time = atoi(argv[2]);
        if (round_time == 0) {
            fprintf(stderr, "Invalid argument [round time]\n");
            return EINVAL;
        }
        if (argc >= 4) headless = 1;
    }

    server_context context;
    syn_buffer event_buffer;

    size_t close_count = SERVER_CLOSE_TIME;

    struct sigaction cancel_action;
    cancel_action.sa_handler = cancel;
    cancel_action.sa_flags = 0;
    sigemptyset(&cancel_action.sa_mask);
    sigaction(SIGINT, &cancel_action, NULL);

    int res = socket_server_start(&context.server, port, on_recv, &event_buffer);
    if (res != 0) {
        if (!headless) fprintf(stderr, "%s\n", strerror(errno));
        return errno;
    }
    if (!headless) fprintf(stdout, "Server started on port: %d\n", port);

    context.game = malloc(sizeof(game));
    game_init(context.game);
    pthread_t game_thread;
    pthread_t timer_thread;

    syn_buffer_init(&event_buffer, 16, sizeof(server_event));
    while (running) {
        server_event message;
        message.type = -1;
        syn_buffer_timed_get(&event_buffer, &message);
        if (!running) break;
        if (message.type == -1) continue;
        if (message.type == SERVER_EVENT_RECV) {
            server_recv_data* recv_data = message.data;
            handle_command(recv_data->con_id, recv_data->data, recv_data->len, &context);
            if (!context.game->started && context.game->ready_count > 0 &&
                context.game->ready_count == sll_get_size(&context.game->players)) {
                context.game->time = round_time;
                context.game->started = 1;
                pthread_create(&game_thread, NULL, game_loop, &event_buffer);
                pthread_create(&timer_thread, NULL, timer_tick, &event_buffer);
                command_start p = (command_start){ COMMAND_START, context.game->time};
                broadcast_data(&context.server, &p, sizeof(command_player));
            }
            if (!context.game->started && context.client_count == 0) {
                if (!headless) fprintf(stdout, "Server is empty, closing in: %d seconds\n", SERVER_CLOSE_TIME);
                pthread_create(&timer_thread, NULL, timer_tick, &event_buffer);
            }
        }
        if (message.type == SERVER_EVENT_CIRCLE) {
            command_circle* c = message.data;
            broadcast_data(&context.server, c, sizeof(command_player));
        }
        if (message.type == SERVER_TIMER_TICK) {
            if (!context.game->started && context.client_count >= 0) {
                if (context.client_count == 0) {
                    close_count--;
                    if (close_count == 0) {
                        pthread_cancel(timer_thread);
                        pthread_join(timer_thread, NULL);
                        if (!headless) fprintf(stdout, "Server timeout [%ds] reached, closing\n", SERVER_CLOSE_TIME);
                        break;
                    }
                }
                else {
                    if (!headless) fprintf(stdout, "Someone connected, server close cancelled\n");
                    pthread_cancel(timer_thread);
                    pthread_join(timer_thread, NULL);
                    close_count = SERVER_CLOSE_TIME;
                }
            }

            command_time p = (command_time){ COMMAND_TIME, --context.game->time};
            broadcast_data(&context.server, &p, sizeof(command_time));
            if (context.game->time <= 0) {
                pthread_cancel(game_thread);
                pthread_cancel(timer_thread);
                pthread_join(game_thread, NULL);
                pthread_join(timer_thread, NULL);
                game_clear(context.game);
                command_simple c = (command_simple){ COMMAND_END };
                broadcast_data(&context.server, &c, sizeof(command_simple));
            }
        }
    }
    game_free(context.game);
    return 0;
}