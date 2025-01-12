#ifndef LIST_MENU_H
#define LIST_MENU_H

#include "menu.h"
#include "../../libstructures/sll.h"

#define MENU_TYPE_LIST 2
#define LIST_MENU_WIDTH 8
#define LIST_MENU_HEIGHT 10

void list_menu_init(menu* menu, char* title, sll* list, pthread_mutex_t* mut, void(*fun)(void* obj, char* out));
void list_init_options(menu* menu, int count, ...);

#endif
