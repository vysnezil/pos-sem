#include "object.h"
#include <stdlib.h>

void add_object(object_context* context, object* obj) {
    obj->object_context = context;
    sll_add(&context->objects, obj);
    graphics_refresh(context->graphics, context);
}

void remove_object(object_context* context, int object_id) {
    const size_t size = sll_get_size(&context->objects);
    for (int i = 0; i < size; i++) {
        object out;
        sll_get(&context->objects, i, &out);
        if (out.id == object_id) {
            sll_remove(&context->objects, i);
            break;
        }
    }
    graphics_refresh(context->graphics, context);
}

_Bool objects_click(object_context* context, int x, int y) {
    //pthread_mutex_lock(&context->mutex);
    char clicked_id = context->screen[y * context->screen_width + x];
    //pthread_mutex_unlock(&context->mutex);
    if (clicked_id == (char)0xFF) return 0;
    const size_t size = sll_get_size(&context->objects);
    object out;
    for (int i = 0; i < size; i++) {
        sll_get(&context->objects, i, &out);
        if (out.id == clicked_id) {
            if (out.on_click == NULL) return 1;
            out.on_click(&out, out.context);
            sll_set(&context->objects, i, &out);
            break;
        }
    }
    graphics_refresh(context->graphics, context);
    return 1;
}

void free_object(object* obj) {
    free(obj->data);
}

void object_context_init(object_context* context, graphics_context* graphics) {
    context->graphics = graphics;
    context->screen = NULL;
    pthread_mutex_init(&context->mutex, NULL);
    sll_init(&context->objects, sizeof(object));
}

void object_context_free(object_context* context) {
    sll_destroy(&context->objects);
    if (context->screen != NULL) free(context->screen);
    pthread_mutex_destroy(&context->mutex);
}

void object_screen_resize(object_context* context, int w, int h) {
    pthread_mutex_lock(&context->mutex);
    context->screen_width = w;
    context->screen_height = h;
    if (context->screen == NULL) context->screen = malloc(w * h * sizeof(char));
    else {
        char* new_screen = realloc(context->screen, w * h * sizeof(char));
        if (new_screen == NULL) return;
        context->screen = new_screen;
    }
    for (size_t i = 0; i < w * h; i++) context->screen[i] = (char)0xFF;
    pthread_mutex_unlock(&context->mutex);
    graphics_refresh(context->graphics, context);
}