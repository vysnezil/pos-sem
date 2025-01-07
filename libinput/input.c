#include "input.h"
#include <pthread.h>

pthread_t input_thread;

typedef struct input_thread_data {
    void (*key_callback)(int,int,void*);
    void* context;
} input_thread_data;

void* handle_input(void* arg) {
    input_thread_data data = *(input_thread_data*)arg;
    free(arg);
    while (1) {
        struct tb_event ev;
        tb_poll_event(&ev);
        switch (ev.type) {
            case TB_EVENT_KEY:
                (*data.key_callback)(ev.ch, ev.key, data.context);
                break;
            default:
                break;
        }
    }
    return NULL;
}

void input_init(void (*callback)(int,int,void*), void* context) {
    tb_init();
    tb_set_cursor(0,0);
    input_thread_data* t_data = malloc(sizeof(input_thread_data));
    t_data->key_callback = callback;
    t_data->context = context;
    pthread_create(&input_thread, NULL, &handle_input, t_data);
}

void input_destroy() {
    pthread_cancel(input_thread);
    pthread_join(input_thread, NULL);
    tb_shutdown();
}
