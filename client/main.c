#include <pthread.h>

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

    graphics_context context;
    graphics_init(&context);

    input_init(&input_on_key, &event_buffer);

    shape c1 = OBJECT_CIRCLE(0, 2, 10, 10, 3);
    shape c2 = OBJECT_CIRCLE(2, 2, 12, 10, 2)
    shape c3 = OBJECT_CIRCLE(1, 2, 15, 13, 4);
    add_object(&context, &c1);
    add_object(&context, &c2);

    menu m = MENU("testmenu");
    menu_init(&m, 0);
    menu_show(&context, &m);

    while (1) {
        int data;
        syn_buffer_get(&event_buffer, (void*)&data);
        if (data == 'q') break;
        add_object(&context, &c3);
        remove_object(&context, 0);
        menu_hide(&context);
    }

    menu_destroy(&m);
    graphics_destroy(&context);
    input_destroy();
    syn_buffer_free(&event_buffer);
    return 0;
}
