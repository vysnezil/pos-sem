#include "object.h"
#include <stdlib.h>

void add_object(object_context* context, object* obj) {
    pthread_mutex_lock(&context->mutex);
    obj->object_context = context;
    sll_add(&context->objects, obj);
    graphics_refresh(context->graphics, context);
    pthread_mutex_unlock(&context->mutex);
}

void remove_object(object_context* context, int object_id) {
    pthread_mutex_lock(&context->mutex);
    const size_t size = sll_get_size(&context->objects);
    for (int i = 0; i < size; i++) {
        object* out = malloc(sizeof(object));
        sll_get(&context->objects, i, out);
        if (out->id == object_id) {
            out->color = COLOR_DEFAULT;
            graphics_force_draw(context->graphics, out);
            sll_remove(&context->objects, i);
            break;
        }
        free(out);
    }
    graphics_refresh(context->graphics, context);
    pthread_mutex_unlock(&context->mutex);
}

_Bool objects_click(object_context* context, int x, int y) {
    pthread_mutex_lock(&context->mutex);
    if (x < 0 || x >= context->screen_width || y < 0 || y >= context->screen_height) {
        pthread_mutex_unlock(&context->mutex);
        return 1;
    }
    char clicked_id = context->screen[y * context->screen_width + x];
    if (clicked_id == (char)0xFF) {
        pthread_mutex_unlock(&context->mutex);
        return 0;
    }
    const size_t size = sll_get_size(&context->objects);
    for (int i = 0; i < size; i++) {
        object* out = sll_get_ref(&context->objects, i);
        if (out != NULL && out->id == clicked_id) {

            if (out->on_click == NULL) return 1;
            out->on_click(out, out->context);
            break;
        }
    }
    graphics_refresh(context->graphics, context);
    pthread_mutex_unlock(&context->mutex);
    return 1;
}

void free_object(object* obj) {
    if (obj->data != NULL) free(obj->data);
}

void s_obj_free(void* obj, void* data) {
    free_object(obj);
}

void object_context_init(object_context* context, graphics_context* graphics) {
    context->graphics = graphics;
    context->screen = NULL;
    pthread_mutex_init(&context->mutex, NULL);
    sll_init(&context->objects, sizeof(object));
}

void object_context_free(object_context* context) {
    sll_for_each(&context->objects, s_obj_free, NULL);
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
        if (new_screen != NULL) context->screen = new_screen;
    }
    for (size_t i = 0; i < w * h; i++) context->screen[i] = (char)0xFF;
    graphics_refresh(context->graphics, context);
    pthread_mutex_unlock(&context->mutex);
}