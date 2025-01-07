#ifndef MENU_H
#define MENU_H

#define MENU_TYPE_NOT_INIT -1

// avoid cyclic dependency
struct graphics_context;

typedef struct menu {
    void* data;
    int type;
    void(*renderer)(struct graphics_context*, struct menu*);
    void(*free_func)(struct menu*);
    void(*on_key)(struct menu*, int key, int ch);
    char* title;
} menu;

#define MENU(name) (menu){.title = name, .type = MENU_TYPE_NOT_INIT};

void menu_destroy(menu* menu);
void menu_input_key(menu* menu, int key, int ch);

#endif
