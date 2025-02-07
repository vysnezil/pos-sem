#include "menu.h"
#include <stddef.h>

void menu_destroy(menu* menu) {
    menu->free_func(menu);
}

void menu_input_key(menu* menu, int key, int ch) {
    if (menu == NULL) return;
    menu->on_key(menu, key, ch);
}