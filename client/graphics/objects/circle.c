#include "circle.h"
#include <stdlib.h>
#include "../../../libdraw/draw.h"

void set_circle_pixel(int x, int y, void* context) {
    object* obj = (object*)context;
    draw_pixel(x, y, obj->color);
    char pix = (obj->color == COLOR_DEFAULT) ? (char)0xFF : (char)obj->id;
    object_context* con = obj->object_context;
    if (x < 0 || (x*2+1) >= con->screen_width || y < 0 || y >= con->screen_height) return;
    con->screen[y * con->screen_width + x*2] = pix;
    con->screen[y * con->screen_width + (x*2+1)] = pix;
}

void circle_render(graphics_context* context, object* object) {
    draw_circle(object->x, object->y, *(int*)object->data, set_circle_pixel, object);
}

void circle_init(object* object, int id, int x, int y, int color, int radius, void(*on_click)(struct object* object, void* context), void* context) {
    object->renderer = circle_render;
    object->on_click = on_click;
    object->color = color;
    object->x = x;
    object->y = y;
    object->id = id;
    object->data = malloc(sizeof(int));
    object->context = context;
    *(int*)object->data = radius;
}