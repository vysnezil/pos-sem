#include "graphics.h"
#include "../libshared/term.h"

void graphics_init() {
    term_init();
}

void graphics_destroy() {
    term_destroy();
}

void graphics_refresh() {
    term_update();
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
    term_draw(x, y, 'x', 2, 0);
}