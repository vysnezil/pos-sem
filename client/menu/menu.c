#include "menu.h"
#include <stdarg.h>
#include <stdlib.h>
#include "../../libinput/input.h"

void menu_init(menu* menu, int option_count, ...) {
    menu->option_count = option_count;
    va_list arg;
    va_start(arg, option_count);
    menu->options = calloc(option_count, sizeof(menu_option));
    for (int i = 0; i < option_count; i++) {
        menu->options[i] = va_arg(arg, menu_option*);
    }
    va_end(arg);
    menu->selected = 0;
}

void menu_destroy(menu* menu) {
    free(menu->options);
    menu->options = NULL;
    // set menu as destroyed, to stop render on render thread when menu is destroyed
    menu->option_count = -1;
}

void menu_input_key(menu* menu, int key, int ch) {
    switch (key) {
        case KEY_UP:
            menu->selected++;
            if (menu->selected >= menu->option_count) menu->selected = 0;
            break;
        case KEY_DOWN:
            menu->selected--;
            if (menu->selected < 0) menu->selected = menu->option_count - 1;
            break;
        case KEY_ENTER:
            if (menu->options[menu->selected]->on_select != NULL)
                menu->options[menu->selected]->on_select(menu->options[menu->selected]->data);
            break;
        default:
            break;
    }
}