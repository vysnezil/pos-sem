#ifndef DRAW_H
#define DRAW_H

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

#define OBJECT_CIRCLE(id, color, x, y, r) (shape){ id, SHAPE_CIRCLE, color, x, y, r, PARAM_NULL };

void add_object(shape* sh);
void remove_object(int shape_id);

void graphics_init();
void graphics_destroy();
void graphics_refresh();

void graphics_interact_mouse(int x, int y);
void graphics_interact_key(int key, int ch);

int get_width();
int get_height();

#endif
