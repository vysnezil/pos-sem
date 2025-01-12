#ifndef MESSAGE_MENU_H
#define MESSAGE_MENU_H
#include "menu.h"

#define MENU_TYPE_MESSAGE 3

void message_menu_init(char* text, menu* menu, void(*callback)(void*), void* context);

#endif
