#include "menu.h"
#include <stdarg.h>
#include <stdlib.h>

void menu_init(menu* menu, int option_count, ...) {
    va_list arg;
    va_start(arg, option_count);
    menu->options = calloc(option_count, sizeof(menu_option));
    for (int i = 0; i < option_count; i++) {
        menu->options[i] = va_arg(arg, menu_option*);
    }
    va_end(arg);
}

void menu_destroy(menu* menu) {
    free(menu->options);
}