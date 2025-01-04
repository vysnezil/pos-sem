#include "input.h"
#include "../libshared/termbox2.h"
#include <pthread.h>

pthread_t input_thread;

typedef struct thread_data {

} thread_data;

void* handle_input(void* arg) {

    return NULL;
}

void input_init() {
    tb_init();
    pthread_create(&input_thread, NULL, &handle_input, NULL);
    pthread_detach(input_thread);
}

void input_destroy() {
    pthread_cancel(input_thread);
    tb_shutdown();
}

void input_getkey() {
    struct tb_event ev;
    tb_poll_event(&ev);
}