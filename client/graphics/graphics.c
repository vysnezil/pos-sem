#include "graphics.h"

#include <stdlib.h>
#include <string.h>

#include "../../libdraw/draw.h"
#include "../../libshared/termbox2.h"

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

void redraw_shape(const shape * shape) {
    switch (shape->type) {
        case SHAPE_PIXEL:
            draw_pixel(shape->x, shape->y, shape->color);
        break;
        case SHAPE_CIRCLE:
            draw_circle(shape->x, shape->y, shape->param1, shape->color);
        break;
        default:
            break;
    }
}

void draw_shape(void* shape) {
    redraw_shape(shape);
}

void m_add_object(graphics_context* context, shape* sh) {
    sll_add(&context->objects, sh);
    draw_shape(sh);
    redraw_screen(context);
}

void m_remove_object(graphics_context* context, int shape_id) {
    const size_t size = sll_get_size(&context->objects);
    for (int i = 0; i < size; i++) {
        shape out;
        sll_get(&context->objects, i, &out);
        if (out.id == shape_id) {
            sll_remove(&context->objects, i);
            out.color = COLOR_DEFAULT;
            redraw_shape(&out);
        }
    }
    sll_for_each(&context->objects, draw_shape);
    redraw_screen(context);
}

void* handle_render(void* arg) {
    graphics_context* context = arg;
    while (1) {
        render_message m;
        syn_buffer_get(&context->buffer, &m);
        switch (m.type) {
            case M_ADD_SHAPE:
                m_add_object(context, m.data);
                break;
            case M_DEL_SHAPE:
                m_remove_object(context, *(int*)m.data);
                free(m.data);
                break;
            case M_REDRAW:
                tb_clear();
                sll_for_each(&context->objects, draw_shape);
                redraw_screen(context);
                break;
            case M_MENU:
                pthread_mutex_lock(&context->menu_mutex);
                context->active_menu = m.data;
                if (m.data == NULL) {
                    int w = get_width()>>1, h = get_height();
                    draw_rectangle(0, 0, w, h, COLOR_DEFAULT, 1);
                    sll_for_each(&context->objects, draw_shape);
                }
                redraw_screen(context);
                pthread_mutex_unlock(&context->menu_mutex);
            break;
            default:
                break;
        }
    }
    return NULL;
}

void graphics_init(graphics_context* context) {
    syn_buffer_init(&context->buffer, 16, sizeof(render_message));
    sll_init(&context->objects, sizeof(shape));
    pthread_mutex_init(&context->menu_mutex, NULL);
    context->active_menu = NULL;
    draw_init();
    pthread_create(&context->render_thread, NULL, &handle_render, context);
}

void graphics_destroy(graphics_context* context) {
    menu_hide(context);
    pthread_cancel(context->render_thread);
    pthread_join(context->render_thread, NULL);
    sll_destroy(&context->objects);
    syn_buffer_free(&context->buffer);
    pthread_mutex_destroy(&context->menu_mutex);
    draw_destroy();
}

void add_object(graphics_context* context, shape* sh) {
    render_message m = (render_message){sh, M_ADD_SHAPE};
    syn_buffer_add(&context->buffer, &m);
}

void remove_object(graphics_context* context, int object_id) {
    int* obj_id = malloc(sizeof(int));
    *obj_id = object_id;
    render_message m = (render_message){obj_id, M_DEL_SHAPE};
    syn_buffer_add(&context->buffer, &m);
}

void graphics_refresh(graphics_context* context) {
    render_message m = (render_message){NULL, M_REDRAW};
    syn_buffer_add(&context->buffer, &m);
}

int get_width() {
    return draw_get_width();
}

int get_height() {
    return draw_get_height();
}