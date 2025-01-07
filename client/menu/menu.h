#ifndef MENU_H
#define MENU_H
#include <stdatomic.h>

typedef struct menu_option {
    char* text;
    _Bool selectable;
    void (*on_select)(void*);
    void* data;
} menu_option;

typedef struct menu {
    char* title;
    int option_count;
    menu_option** options;
    atomic_int selected;
} menu;

#define MENU(name) (menu){name, 0, NULL};

void menu_init(menu* menu, int option_count, ...);
void menu_destroy(menu* menu);
void menu_input_key(menu* menu, int key, int ch);

#endif
