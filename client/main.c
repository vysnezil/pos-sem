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

    shape c1 = OBJECT_CIRCLE(0, COLOR_RED, 10, 10, 3);
    shape c2 = OBJECT_CIRCLE(2, COLOR_RED | COLOR_BRIGHT, 12, 10, 2)
    shape c3 = OBJECT_CIRCLE(1, COLOR_RED, 15, 13, 4);
    add_object(&context, &c1);
    add_object(&context, &c2);

    menu m = MENU("testmenu");
    menu_option opt = (menu_option){"  ONE         ", 0, NULL};
    menu_option opt2 = (menu_option){"  TWO OPT     ", 1, NULL};
    menu_option opt3 = (menu_option){"  THREEOPTED  ", 0, NULL};
    menu_init(&m, 3, &opt, &opt2, &opt3);

    while (1) {
        int data;
        syn_buffer_get(&event_buffer, (void*)&data);
        if (data == 'q') break;
        if (data == 'm') menu_show(&context, &m);
        if (data == 'e') menu_hide(&context);
        add_object(&context, &c3);
        remove_object(&context, 0);
    }

    menu_destroy(&m);
    graphics_destroy(&context);
    input_destroy();
    syn_buffer_free(&event_buffer);
    return 0;
}
