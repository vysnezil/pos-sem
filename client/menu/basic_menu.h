#ifndef BASIC_MENU_H
#define BASIC_MENU_H
#include "menu.h"
#include <stdatomic.h>

#define MENU_TYPE_BASIC 0
#define MENU_TYPE_BASIC 0

#define MENU_LABEL(text) (menu_option){text, 0, NULL, NULL};

void basic_menu_init(menu* menu, int option_count, ...);

#endif
