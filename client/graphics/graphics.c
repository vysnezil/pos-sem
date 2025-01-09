#include "graphics.h"

#include <stdlib.h>

#include "../../libdraw/draw.h"
#include "../../libshared/termbox2.h"
#include "../../libstructures/sll.h"

#include "object.h"

#define M_ADD_SHAPE 0
#define M_DEL_SHAPE 1
#define M_REDRAW 2
#define M_MENU 3

typedef struct render_message {
    void* data;
    char type;
} render_message;

void redraw_screen(graphics_context* context) {
    tb_hide_cursor();
    if (context->active_menu != NULL) {
        if (context->active_menu->data != NULL) {
            context->active_menu->renderer(context, context->active_menu);
        }
    }
    draw_update();
}

void menu_show(graphics_context* context, menu* menu) {
    menu_hide(context);
    render_message m = (render_message){menu, M_MENU};
    syn_buffer_add(&context->buffer, &m);
}

void menu_hide(graphics_context* context) {
    render_message m = (render_message){NULL, M_MENU};
    syn_buffer_add(&context->buffer, &m);
}

void* handle_render(void* arg) {
    graphics_context* context = arg;
    while (1) {
        render_message m;
        syn_buffer_get(&context->buffer, &m);
        switch (m.type) {
            case M_REDRAW:
                tb_clear();
                object_context* obj = m.data;
                if (obj != NULL) {
                    pthread_mutex_lock(&obj->mutex);
                    const size_t size = sll_get_size(&obj->objects);
                    for (int i = 0; i < size; i++) {
                        object out;
                        sll_get(&obj->objects, i, &out);
                        // passing a copy should be fine
                        out.renderer(obj->graphics, &out);
                    }
                    pthread_mutex_unlock(&obj->mutex);
                }
                redraw_screen(context);
                break;
            case M_MENU:
                pthread_mutex_lock(&context->menu_mutex);
                context->active_menu = m.data;
                if (m.data == NULL) {
                    int w = get_width()>>1, h = get_height();
                    draw_rectangle(0, 0, w, h, COLOR_DEFAULT, 1);
                }
                pthread_mutex_unlock(&context->menu_mutex);
                redraw_screen(context);
            break;
            default:
                break;
        }
    }
    return NULL;
}

void graphics_init(graphics_context* context) {
    syn_buffer_init(&context->buffer, 16, sizeof(render_message));
    pthread_mutex_init(&context->menu_mutex, NULL);
    context->active_menu = NULL;
    draw_init();
    pthread_create(&context->render_thread, NULL, &handle_render, context);
}

void graphics_destroy(graphics_context* context) {
    menu_hide(context);
    pthread_cancel(context->render_thread);
    pthread_join(context->render_thread, NULL);
    syn_buffer_free(&context->buffer);
    pthread_mutex_destroy(&context->menu_mutex);
    draw_destroy();
}

// objects is object context
void graphics_refresh(graphics_context* context, void* objects) {
    render_message m = (render_message){objects, M_REDRAW};
    syn_buffer_add(&context->buffer, &m);
}

int get_width() {
    return draw_get_width();
}

int get_height() {
    return draw_get_height();
}