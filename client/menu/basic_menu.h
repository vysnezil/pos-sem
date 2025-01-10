#ifndef BASIC_MENU_H
#define BASIC_MENU_H
#include "menu.h"
#include <stdatomic.h>

#define MENU_TYPE_BASIC 0

typedef struct menu_option {
    char* text;
    _Bool selectable;
    void (*on_select)(void*);
    void* data;
} menu_option;

#define MENU_LABEL(text) (menu_option){text, 0, NULL, NULL};

void basic_menu_init(menu* menu, int option_count, ...);

#endif
