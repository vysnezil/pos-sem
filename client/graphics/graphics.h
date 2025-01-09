#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <pthread.h>
#include "../../libstructures/syn_buffer.h"
#include "../menu/menu.h"

#define COLOR_DEFAULT 0
#define COLOR_BLACK 0x2000
#define COLOR_RED 1
#define COLOR_GREEN 2
#define COLOR_YELLOW 3
#define COLOR_BLUE 4
#define COLOR_MAGENTA 5
#define COLOR_CYAN 6
#define COLOR_WHITE 7

#define COLOR_BRIGHT 8
#define COLOR_REVERSE 0x400

typedef struct graphics_context {
  pthread_t render_thread;
  syn_buffer buffer;
  menu* active_menu;
  pthread_mutex_t menu_mutex;
} graphics_context;

void graphics_init(graphics_context* context);
void graphics_destroy(graphics_context* context);
// objects is object context
void graphics_refresh(graphics_context* context, void* objects);
// obj, object to draw with context set
void graphics_force_draw(graphics_context* context, void* obj);

void menu_show(graphics_context* context, menu* menu);
void menu_hide(graphics_context* context);

int get_width();
int get_height();

#endif
