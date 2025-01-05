#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include "libstructures/syn_buffer.h"
#include "libgraphics/graphics.h"
#include "libinput/input.h"

void on_key(int ch, int key, void* context) {
    syn_buffer* event_buffer = context;
    syn_buffer_add(event_buffer, &ch);
}

int main() {
    syn_buffer event_buffer;
    syn_buffer_init(&event_buffer, 16, sizeof(int));

    graphics_init();
    input_init(&on_key, &event_buffer);

    //draw_pixel(15, 10);
    //draw_line(10, 10, 20, 20);
    //draw_circle(10, 15, 3);
    //graphics_refresh();

    while (1) {
        int data;
        syn_buffer_get(&event_buffer, (void*)&data);
        if (data == 'q') break;
        draw_char(30, 10, data);
        graphics_refresh();
    }

    input_destroy();
    graphics_destroy();
    syn_buffer_free(&event_buffer);
    return 0;
}
