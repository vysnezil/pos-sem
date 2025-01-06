#include "graphics.h"
#include "../../libdraw/draw.h"

#include "../../libstructures/syn_buffer.h"
#include "../../libstructures/sll.h"

typedef struct render_thread_data {
    syn_buffer buff;
} render_thread_data;

//pthread_t render_thread;
sll objects;

void* handle_render(void* arg) {
    //render thread, object click handler, menu object, keyboard handler)
}

void redraw_shape(const shape * shape, const _Bool clear) {
    int color = clear ? 0 : shape->color;
    switch (shape->type) {
        case SHAPE_PIXEL:
            draw_pixel(shape->x, shape->y, color);
            break;
        case SHAPE_CIRCLE:
            draw_circle(shape->x, shape->y, shape->param1, color);
            break;
        default:
            break;
    }
}

void draw_shape(void* shape) {
    redraw_shape(shape, 0);
}

void graphics_init() {
    draw_init();
    sll_init(&objects, sizeof(shape));
    //pthread_create(&render_thread, NULL, &handle_render, NULL);
}

void graphics_destroy() {
    draw_destroy();
    sll_clear(&objects);
}

void graphics_refresh() {
    draw_update();
}

void add_object(shape* sh) {
    sll_add(&objects, sh);
    draw_shape(sh);
    graphics_refresh();
}

void remove_object(int shape_id) {
    const size_t size = sll_get_size(&objects);
    for (int i = 0; i < size; i++) {
        shape out;
        sll_get(&objects, i, &out);
        if (out.id == shape_id) {
            sll_remove(&objects, i);
            redraw_shape(&out, 1);
        }
    }
    sll_for_each(&objects, draw_shape);
    graphics_refresh();
}

int get_width() {
    return draw_get_width();
}

int get_height() {
    return draw_get_height();
}