#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "../libinput/input.h"
#include "../libstructures/syn_buffer.h"
#include "graphics/graphics.h"

void input_on_key(int ch, int key, void* context) {
    syn_buffer* event_buffer = context;
    syn_buffer_add(event_buffer, &ch);
}

int main() {
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(int));

    graphics_init();
    input_init(&input_on_key, &event_buffer);

    shape c1 = OBJECT_CIRCLE(0, 2, 10, 10, 3);
    shape c2 = OBJECT_CIRCLE(2, 2, 12, 10, 2)
    shape c3 = OBJECT_CIRCLE(1, 2, 15, 13, 4);
    add_object(&c1);
    add_object(&c2);

    while (1) {
        int data;
        syn_buffer_get(&event_buffer, (void*)&data);
        if (data == 'q') break;
        add_object(&c3);
        remove_object(0);
    }

    graphics_destroy();
    syn_buffer_free(&event_buffer);
    return 0;
}
