#include "draw.h"
#include "../libshared/termbox2.h"

void draw_init() {
    tb_init();
    tb_set_output_mode(TB_OUTPUT_256);
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

void draw_line(int x1, int y1, int x2, int y2, int fg_color, int bg_color, int ch) {
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
        draw_char(x1*2, y1, ch, fg_color, bg_color);
        draw_char(x1*2+1, y1, ch, fg_color, bg_color);
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
    tb_set_cell(x*2, y, ' ', color, color);
    tb_set_cell(x*2+1, y, ' ', color, color);
}
void draw_char(int x, int y, int character, int fg_color, int bg_color) {
    tb_set_cell(x, y, character, fg_color, bg_color);
}

void draw_circle(int x, int y, int r, void(*draw)(int x, int y, void*), void* context) {
    int dx = 0;
    int dy = r;
    int p = 1 - r;

    draw(x, y + r, context);
    draw(x, y - r, context);
    draw(x + r, y, context);
    draw(x - r, y, context);

    while (dx < dy) {
        dx++;
        if (p < 0) {
            p = p + 2 * dx + 1;
        }
        else {
            dy--;
            p = p + 2 * (dx - dy) + 1;
        }

        draw(x + dx, y + dy, context);
        draw(x - dx, y + dy, context);
        draw(x + dx, y - dy, context);
        draw(x - dx, y - dy, context);
        draw(x + dy, y + dx, context);
        draw(x - dy, y + dx, context);
        draw(x + dy, y - dx, context);
        draw(x - dy, y - dx, context);
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
        draw_line(x1, y1, x2, y1, color, color, ' ');
        draw_line(x1, y1, x1, y2, color, color, ' ');
        draw_line(x1, y2, x2, y2, color, color, ' ');
        draw_line(x2, y1, x2, y2, color, color, ' ');
    }
}

void draw_text(int x, int y, char *text, int fg_color, int bg_color, _Bool plusone) {
    tb_print((x*2) + (plusone ? 1 : 0), y, fg_color, bg_color, text);
}