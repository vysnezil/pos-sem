#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <pthread.h>
#include "../../libstructures/syn_buffer.h"
#include "../../libstructures/sll.h"

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
} graphics_context;

#define OBJECT_CIRCLE(id, color, x, y, r) (shape){ id, SHAPE_CIRCLE, color, x, y, r, PARAM_NULL };

void add_object(graphics_context* context, shape* sh);
void remove_object(graphics_context* context, int shape_id);

void graphics_init(graphics_context* context);
void graphics_destroy(graphics_context* context);
void graphics_refresh(graphics_context* context);

int get_width();
int get_height();

#endif
