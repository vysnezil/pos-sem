#ifndef MENU_H
#define MENU_H

typedef struct menu_option {
    char* text;
    _Bool selectable;
    void* on_select;
} menu_option;

typedef struct menu {
    char* title;
    int option_count;
    menu_option** options;
} menu;

#define MENU(name) (menu){name, 0, NULL};

void menu_init(menu* menu, int option_count, ...);
void menu_destroy(menu* menu);

#endif
