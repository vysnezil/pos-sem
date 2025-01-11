#ifndef OBJECT_H
#define OBJECT_H

#include "../../libstructures/sll.h"
#include "graphics.h"

typedef struct object_context {
    sll objects;
    char* screen;
    int screen_width, screen_height;
    graphics_context* graphics;
    pthread_mutex_t mutex;
} object_context;

typedef struct object {
    int id, x, y;
    int color;
    void* data;
    void(*renderer)(graphics_context*, struct object* object);
    void(*on_click)(struct object* object, void* context);
    void* context;
    object_context* object_context;
} object;

void object_context_init(object_context* context, graphics_context* graphics);
void object_context_free(object_context* context);
void add_object(object_context* context, object* obj);
void remove_object(object_context* context, int object_id);
void objects_clear(object_context* context);
_Bool objects_click(object_context* context, int x, int y);
void object_screen_resize(object_context* context, int w, int h);
void free_object(object* obj);

#endif
