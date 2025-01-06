#include "graphics.h"
#undef TB_IMPL
#include "graphics_draw.h"
#include "../libshared/termbox2.h"

#include "../libstructures/syn_buffer.h"
#include "../libstructures/sll.h"

typedef struct render_thread_data {
    syn_buffer buff;
} render_thread_data;

//pthread_t render_thread;
sll objects;

void* handle_render(void* arg) {

}

void redraw_shape(const shape * shape, const _Bool clear) {
    int color = clear ? TB_BLACK : shape->color;
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
    tb_init();
    sll_init(&objects, sizeof(shape));
    //pthread_create(&render_thread, NULL, &handle_render, NULL);
}

void graphics_destroy() {
    tb_shutdown();
    sll_clear(&objects);
}

void graphics_refresh() {
    tb_present();
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
        redraw_shape(&out, 1);
        if (out.id == shape_id) {
            sll_remove(&objects, i);
        }
    }
    sll_for_each(&objects, draw_shape);
    graphics_refresh();
}

int get_width() {
    return tb_width();
}

int get_height() {
    return tb_height();
}