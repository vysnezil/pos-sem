#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <pthread.h>
#include "../../libstructures/syn_buffer.h"
#include "../../libstructures/sll.h"
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

#define PARAM_NULL 0xFF

#define SHAPE_PIXEL 0
#define SHAPE_LINE 1
#define SHAPE_CIRCLE 2
#define SHAPE_RECTANGLE 3

typedef struct shape {
  int id;
  int type;
  int color;
  int x;
  int y;
  int param1;
  int param2;
} shape;

typedef struct graphics_context {
  pthread_t render_thread;
  sll objects;
  syn_buffer buffer;
  menu* active_menu;
  pthread_mutex_t menu_mutex;
} graphics_context;

#define OBJECT_CIRCLE(id, color, x, y, r) (shape){ id, SHAPE_CIRCLE, color, x, y, r, PARAM_NULL };

void add_object(graphics_context* context, shape* sh);
void remove_object(graphics_context* context, int object_id);

void graphics_init(graphics_context* context);
void graphics_destroy(graphics_context* context);
void graphics_refresh(graphics_context* context);

void menu_show(graphics_context* context, menu* menu);
void menu_hide(graphics_context* context);

int get_width();
int get_height();

#endif
