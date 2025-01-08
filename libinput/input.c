#include "input.h"
#include <pthread.h>

pthread_t input_thread;

typedef struct input_thread_data {
    void (*key_callback)(int,int,void*);
    void (*mouse_callback)(int,int,int,void*);
    void (*other_callback)(struct tb_event*, void*);
    void* context;
} input_thread_data;

void* handle_input(void* arg) {
    input_thread_data data = *(input_thread_data*)arg;
    free(arg);
    _Bool mouse_down = 0;
    while (1) {
        struct tb_event ev;
        tb_poll_event(&ev);
        switch (ev.type) {
            case TB_EVENT_KEY:
                (*data.key_callback)(ev.ch, ev.key, data.context);
                break;
            case TB_EVENT_MOUSE:
                if (!mouse_down) {
                    (*data.mouse_callback)(ev.x, ev.y, ev.key, data.context);
                    mouse_down = 1;
                }
                if (ev.key == TB_KEY_MOUSE_RELEASE) mouse_down = 0;
                break;
            default:
                if (data.other_callback != NULL) (*data.other_callback)(&ev, data.context);
                break;
        }
    }
    return NULL;
}

void input_init(void* context,
    void (*key_callback)(int,int,void*),
    void (*mouse_callback)(int,int,int,void*),
    void (*other_callback)(struct tb_event*, void*)) {
    tb_init();
    tb_hide_cursor();
    tb_set_input_mode(TB_INPUT_MOUSE);
    input_thread_data* t_data = malloc(sizeof(input_thread_data));
    t_data->key_callback = key_callback;
    t_data->mouse_callback = mouse_callback;
    t_data->context = context;
    t_data->other_callback = other_callback;
    pthread_create(&input_thread, NULL, &handle_input, t_data);
}

void input_destroy() {
    pthread_cancel(input_thread);
    pthread_join(input_thread, NULL);
    tb_shutdown();
}
