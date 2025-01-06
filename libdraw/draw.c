#include "draw.h"
#include "../libshared/termbox2.h"

void draw_init() {
    tb_init();
}

void draw_destroy() {
    tb_shutdown();
}

void draw_update() {
    tb_present();
}

int draw_get_width() {
    return tb_width();
}

int draw_get_height() {
    return tb_height();
}

void draw_line(int x1, int y1, int x2, int y2, int color) {
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
        draw_pixel(x1, y1, color);
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

void draw_pixel(int x, int y, int color) {
    tb_set_cell(x*2, y, ' ', TB_CYAN, color);
    tb_set_cell(x*2+1, y, ' ', TB_CYAN, color);
}
void draw_char(int x, int y, int character, int color) {
    tb_set_cell(x, y, character, color, TB_BLACK);
}

void draw_circle(int x, int y, int r, int color) {
    int dx = 0;
    int dy = r;
    int p = 1 - r;

    draw_pixel(x, y + r, color);
    draw_pixel(x, y - r, color);
    draw_pixel(x + r, y, color);
    draw_pixel(x - r, y, color);

    while (dx < dy) {
        dx++;
        if (p < 0) {
            p = p + 2 * dx + 1;
        }
        else {
            dy--;
            p = p + 2 * (dx - dy) + 1;
        }

        draw_pixel(x + dx, y + dy, color);
        draw_pixel(x - dx, y + dy, color);
        draw_pixel(x + dx, y - dy, color);
        draw_pixel(x - dx, y - dy, color);
        draw_pixel(x + dy, y + dx, color);
        draw_pixel(x - dy, y + dx, color);
        draw_pixel(x + dy, y - dx, color);
        draw_pixel(x - dy, y - dx, color);
    }
}

void draw_rectangle(int x1, int y1, int x2, int y2, int color, _Bool fill) {
    if (fill) {
        for (int i = y1; i <= y2; i++) {
            for (int j = x1; j <= x2; j++) {
                draw_pixel(j, i, color);
            }
        }
    }
    else {
        draw_line(x1, y1, x2, y1, color);
        draw_line(x1, y1, x1, y2, color);
        draw_line(x1, y2, x2, y2, color);
        draw_line(x2, y1, x2, y2, color);
    }
}