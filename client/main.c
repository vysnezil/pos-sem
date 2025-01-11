#include <pthread.h>

#include "../libinput/input.h"
#include "../libstructures/syn_buffer.h"
#include "graphics/graphics.h"
#include "graphics/object.h"
#include "menu/basic_menu.h"

#include "game/game_handler.h"

#include "client.h"
#include "ui.h"

#define KEY_INPUT_EVENT 0
#define MOUSE_INPUT_EVENT 1
#define RESIZE_EVENT 2
#define CONNECTION_EVENT 3

typedef struct event_message {
    void* data;
    int type;
} event_message;

typedef struct key_input_event_data {
    int key;
    int ch;
} key_input_event_data;

typedef struct mouse_input_event_data {
    int x;
    int y;
    int key;
} mouse_input_event_data;

typedef struct network_event_data {
    void* data;
    size_t size;
} network_event_data;

void input_on_key(int ch, int key, void* context) {
    syn_buffer* event_buffer = context;
    key_input_event_data* event_data = malloc(sizeof(key_input_event_data));
    event_data->key = key;
    event_data->ch = ch;
    event_message m = (event_message) {event_data, KEY_INPUT_EVENT};
    syn_buffer_add(event_buffer, &m);
}

void input_on_mouse(int x, int y, int key, void* context) {
    syn_buffer* event_buffer = context;
    mouse_input_event_data* event_data = malloc(sizeof(mouse_input_event_data));
    event_data->x = x;
    event_data->y = y;
    event_data->key = key;
    event_message m = (event_message) {event_data, MOUSE_INPUT_EVENT};
    syn_buffer_add(event_buffer, &m);
}

void input_on_event(struct tb_event* ev, void* context) {
    if (ev->type == TB_EVENT_RESIZE) {
        event_message m = (event_message) {NULL, RESIZE_EVENT};
        syn_buffer_add(context, &m);
    }
}

void on_network(void* data, size_t size, void* context) {
    network_event_data* network_data = malloc(sizeof(network_event_data));
    network_data->data = data;
    network_data->size = size;
    event_message m = (event_message) {network_data, CONNECTION_EVENT};
    syn_buffer_add(context, &m);
}

int main() {
    main_context context;
    context.on_receive = on_network;
    context.running = 1;
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(event_message));

    graphics_init(&context.graphics);
    object_context_init(&context.objects, &context.graphics);
    object_screen_resize(&context.objects, get_width(), get_height());
    input_init(&context.input, &event_buffer, input_on_key, input_on_mouse, input_on_event);

    show_main_menu(&context);

    while (context.running) {
        event_message message;
        syn_buffer_get(&event_buffer, &message);
        if (message.type == KEY_INPUT_EVENT) {
            key_input_event_data* ev_data = message.data;
            int key = ev_data->key, ch = ev_data->ch;
            free(ev_data);

            pthread_mutex_lock(&context.graphics.menu_mutex);
            if (context.graphics.active_menu != NULL) menu_input_key(context.graphics.active_menu, key, ch);
            else {
                //if (ch == 'm') menu_show(&context.graphics, &m);
            }

            pthread_mutex_unlock(&context.graphics.menu_mutex);
            graphics_refresh(&context.graphics, &context.objects);
        }
        if (message.type == MOUSE_INPUT_EVENT) {
            mouse_input_event_data* ev_data = message.data;
            int x = ev_data->x, y = ev_data->y, key = ev_data->key;
            free(ev_data);
            if (!objects_click(&context.objects, x, y)) {

            }
        }
        if (message.type == CONNECTION_EVENT) {
            network_event_data* ev_data = message.data;
            handle_command(ev_data->data, ev_data->size, &context);
            free(ev_data);
        }
    }

    object_context_free(&context.objects);
    input_destroy(&context.input);
    graphics_destroy(&context.graphics);
    syn_buffer_free(&event_buffer);
    return 0;
}
