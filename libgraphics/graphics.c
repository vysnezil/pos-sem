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
    tb_set_cell(x*2, y, ' ', TB_CYAN, TB_CYAN);
    tb_set_cell(x*2+1, y, ' ', TB_CYAN, TB_CYAN);
}
void draw_char(int x, int y, int character) {
    tb_set_cell(x, y, character, TB_CYAN, TB_BLACK);
}

void draw_circle(int x, int y, int r) {
    int dx = 0;
    int dy = r;
    int p = 1 - r;

    draw_pixel(x, y + r);
    draw_pixel(x, y - r);
    draw_pixel(x + r, y);
    draw_pixel(x - r, y);

    while (dx < dy) {
        dx++;
        if (p < 0) {
            p = p + 2 * dx + 1;
        }
        else {
            dy--;
            p = p + 2 * (dx - dy) + 1;
        }

        draw_pixel(x + dx, y + dy);
        draw_pixel(x - dx, y + dy);
        draw_pixel(x + dx, y - dy);
        draw_pixel(x - dx, y - dy);
        draw_pixel(x + dy, y + dx);
        draw_pixel(x - dy, y + dx);
        draw_pixel(x + dy, y - dx);
        draw_pixel(x - dy, y - dx);
    }
}

int get_width() {
    return tb_width();
}
int get_height() {
    return tb_height();
}

