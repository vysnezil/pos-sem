#include <pthread.h>

#include "../libdraw/draw.h"
#include "../libinput/input.h"
#include "../libstructures/syn_buffer.h"
#include "graphics/graphics.h"
#include "graphics/object.h"
#include "menu/basic_menu.h"
#include "menu/input_menu.h"
#include "graphics/objects/circle.h"

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
    object_context* context;
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
        object_screen_resize(((input_context*)context)->context, ev->x, ev->y);
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

void on_circle(object* obj, void* context) {
    pthread_mutex_unlock(&obj->object_context->mutex);
    remove_object(obj->object_context, obj->id);
}

int main() {
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(event_message));

    graphics_context context;
    graphics_init(&context);

    object_context object_context;
    object_context_init(&object_context, &context);
    object_screen_resize(&object_context, get_width(), get_height());

    input_context i_context = (struct input_context){&event_buffer, &object_context};

    input_init(&i_context, input_on_key, input_on_mouse, input_on_event);

    object circle;
    object circle2;
    circle_init(&circle, 0, 10, 10, COLOR_RED, 4, on_circle, NULL);
    circle_init(&circle2, 1, 12, 10, COLOR_GREEN, 5, on_circle, NULL);

    add_object(&object_context, &circle);
    add_object(&object_context, &circle2);

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
                if (ch == 'e') remove_object(&object_context, 0);

            }
            pthread_mutex_unlock(&context.menu_mutex);

            graphics_refresh(&context, &object_context);
        }
        if (message.type == MOUSE_INPUT_EVENT) {
            mouse_input_event_data* ev_data = message.data;
            int x = ev_data->x, y = ev_data->y, key = ev_data->key;
            free(ev_data);
            if (!objects_click(&object_context, x, y)) {
                object c;
                circle_init(&c, shapez++, x>>1, y, (shapez%6)+1, 3, on_circle, NULL);
                add_object(&object_context, &c);
            }
        }
    }

    menu_destroy(&m);
    object_context_free(&object_context);
    input_destroy();
    graphics_destroy(&context);
    syn_buffer_free(&event_buffer);
    return 0;
}
