#include <pthread.h>

#include "../libinput/input.h"
#include "../libstructures/syn_buffer.h"
#include "graphics/graphics.h"
#include "menu/basic_menu.h"

void on_select(void* data) {
    menu_hide(data);
}

void on_select_exit(void* data) {
    *(_Bool*)data = 0;
}

#define INPUT_EVENT 0

//today: object mouse, menu labels, menu input text, split this, network thread,  (opt menu mouse?),

typedef struct event_message {
    void* data;
    int type;
} event_message;

typedef struct input_event_data {
    int key;
    int ch;
} input_event_data;

void input_on_key(int ch, int key, void* context) {
    syn_buffer* event_buffer = context;
    input_event_data* event_data = malloc(sizeof(input_event_data));
    event_data->key = key;
    event_data->ch = ch;
    event_message m = (event_message) {event_data, INPUT_EVENT};
    syn_buffer_add(event_buffer, &m);
}

int main() {
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(event_message));

    graphics_context context;
    graphics_init(&context);

    input_init(&input_on_key, &event_buffer);

    shape c1 = OBJECT_CIRCLE(0, COLOR_RED, 10, 10, 3);
    shape c2 = OBJECT_CIRCLE(2, COLOR_RED | COLOR_BRIGHT, 12, 10, 2)
    add_object(&context, &c1);
    add_object(&context, &c2);

    _Bool run = 1;

    menu m = MENU("testmenu");
    menu_option opt = (menu_option){"  ONE         ", 0, on_select, &context};
    menu_option opt2 = (menu_option){"  TWO OPT     ", 1, NULL, NULL};
    menu_option opt3 = (menu_option){"  THREEOPTED  ", 0, on_select_exit, &run};
    basic_menu_init(&m, 3, &opt, &opt2, &opt3);
    menu_show(&context, &m);

    while (run) {
        event_message message;
        syn_buffer_get(&event_buffer, (void*)&message);
        if (message.type == INPUT_EVENT) {
            input_event_data* ev_data = message.data;
            int key = ev_data->key, ch = ev_data->ch;
            free(ev_data);
            if (ch == 'q') break;
            if (ch == 'm') menu_show(&context, &m);
            menu_input_key(&m, key, ch);
            graphics_refresh(&context);
        }
    }

    menu_destroy(&m);
    input_destroy();
    graphics_destroy(&context);
    syn_buffer_free(&event_buffer);
    return 0;
}
