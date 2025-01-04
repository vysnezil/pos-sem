#include "graphics.h"
#include "../libshared/termbox2.h"

void graphics_init() {
    tb_init();
}

void graphics_destroy() {
    tb_shutdown();
}

void graphics_refresh() {
    tb_present();
}

void draw_line(int x1, int y1, int x2, int y2) {
    int dx = x2-x1, dy = y2-y1, sx = 1, sy = 1;
    if (x1 > x2) {
        dx = x1-x2;
        sx = -1;
    }
    if (y1 > y2) {
        dy = y1-y2;
        sy = -1;
    }
    int err = dx - dy;

    while (1) {
        draw_pixel(y1, x1);
        if (x1 == x2 && y1 == y2)
            break;

        const int e2 = err << 1;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_pixel(int x, int y) {
    tb_set_cell(x, y, 'x', TB_CYAN, TB_BLACK);
}