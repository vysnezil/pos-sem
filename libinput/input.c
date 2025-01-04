#include "input.h"
#define TB_IMPL
#include "../libshared/term.h"
#include <pthread.h>

pthread_t input_thread;

typedef struct thread_data {

} thread_data;

void* handle_input(void* arg) {

    return NULL;
}

void input_init() {
    term_init();
    pthread_create(&input_thread, NULL, &handle_input, NULL);
    pthread_detach(input_thread);
}

void input_destroy() {
    pthread_cancel(input_thread);
    term_destroy();
}

void input_getkey() {
    term_wait_key();
}