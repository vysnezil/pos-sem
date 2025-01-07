#ifndef INPUT_MENU_H
#define INPUT_MENU_H
#include "menu.h"

#define MENU_TYPE_INPUT 1

typedef struct input_menu_data {
  void(*callback)(char*, void*);
  void* context;
  char* buffer;
  int len;
  int limit;
  char* prompt;
} input_menu_data;

void input_menu_init(char* prompt, menu* menu, void(*callback)(char*, void*), void* context);

#endif
