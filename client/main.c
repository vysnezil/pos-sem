#include <pthread.h>

#include "../libdraw/draw.h"
#include "../libinput/input.h"
#include "../libstructures/syn_buffer.h"
#include "graphics/graphics.h"
#include "menu/basic_menu.h"
#include "menu/input_menu.h"

void on_select(void* data) {
    menu_hide(data);
}

void on_select_exit(void* data) {
    *(_Bool*)data = 0;
}

#define KEY_INPUT_EVENT 0
#define MOUSE_INPUT_EVENT 1

//today: object mouse, split this, game logic, network thread,

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

typedef struct input_context {
    syn_buffer* event_buffer;
    graphics_context* context;
} input_context;

void input_on_key(int ch, int key, void* context) {
    syn_buffer* event_buffer = ((input_context*)context)->event_buffer;
    key_input_event_data* event_data = malloc(sizeof(key_input_event_data));
    event_data->key = key;
    event_data->ch = ch;
    event_message m = (event_message) {event_data, KEY_INPUT_EVENT};
    syn_buffer_add(event_buffer, &m);
}

void input_on_mouse(int x, int y, int key, void* context) {
    syn_buffer* event_buffer = ((input_context*)context)->event_buffer;
    mouse_input_event_data* event_data = malloc(sizeof(mouse_input_event_data));
    event_data->x = x;
    event_data->y = y;
    event_data->key = key;
    event_message m = (event_message) {event_data, MOUSE_INPUT_EVENT};
    syn_buffer_add(event_buffer, &m);
}

void input_on_event(struct tb_event* ev, void* context) {
    if (ev->type == TB_EVENT_RESIZE) {
        graphics_refresh(((input_context*)context)->context);
    }
}

void in_menu_callback(char* data, void* context) {
    graphics_context* d = context;
    menu_destroy(d->active_menu);
    free(d->active_menu);
    menu_hide(context);
}


void menu_call_input(void* data) {
    menu* in = malloc(sizeof(menu));
    in->title = "Inputtest";
    input_menu_init("Enter value:", in, in_menu_callback, data);
    menu_show(data, in);
}

int main() {
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(event_message));

    graphics_context context;
    graphics_init(&context);
    input_context i_context = (struct input_context){&event_buffer, &context};

    input_init(&i_context, input_on_key, input_on_mouse, input_on_event);

    shape c1 = OBJECT_CIRCLE(0, COLOR_RED, 10, 10, 3);
    shape c2 = OBJECT_CIRCLE(2, COLOR_RED | COLOR_BRIGHT, 12, 10, 2)
    add_object(&context, &c1);
    add_object(&context, &c2);

    _Bool run = 1;

    menu m = MENU("testmenu");
    menu_option opt = (menu_option){"    return    ", 1, on_select, &context};
    menu_option opt2 = (menu_option){"  test input  ", 1, menu_call_input, &context};
    menu_option opt3 = MENU_LABEL("label this");
    menu_option opt4 = (menu_option){"     quit     ", 1, on_select_exit, &run};
    basic_menu_init(&m, 4, &opt, &opt2, &opt3, &opt4);
    //menu_show(&context, &m);
    int shapez = 5;
    while (run) {
        event_message message;
        syn_buffer_get(&event_buffer, (void*)&message);
        if (message.type == KEY_INPUT_EVENT) {
            key_input_event_data* ev_data = message.data;
            int key = ev_data->key, ch = ev_data->ch;
            free(ev_data);

            pthread_mutex_lock(&context.menu_mutex);
            if (context.active_menu != NULL) menu_input_key(context.active_menu, key, ch);
            else {
                if (ch == 'q') {
                    pthread_mutex_unlock(&context.menu_mutex);
                    run = 0;
                    break;
                }
                if (ch == 'm') menu_show(&context, &m);
                if (ch == 'e') change_object_color(&context, c1.id, COLOR_GREEN);

            }
            pthread_mutex_unlock(&context.menu_mutex);

            graphics_refresh(&context);
        }
        if (message.type == MOUSE_INPUT_EVENT) {
            mouse_input_event_data* ev_data = message.data;
            int x = ev_data->x, y = ev_data->y, key = ev_data->key;
            free(ev_data);
            shape s = OBJECT_CIRCLE(shapez++, COLOR_RED, x>>1, y, 2);
            add_object(&context, &s);
        }
    }

    menu_destroy(&m);
    input_destroy();
    graphics_destroy(&context);
    syn_buffer_free(&event_buffer);
    return 0;
}
