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

typedef struct basic_menu_data {
    int option_count;
    menu_option** options;
    atomic_uint selected;
} basic_menu_data;

void basic_menu_init(menu* menu, int option_count, ...);

#endif
