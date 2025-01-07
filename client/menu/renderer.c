#include "renderer.h"
#include <string.h>

void render_frame(menu* menu, int width, int height) {
    int w = get_width()>>2, h = get_height()>>1;
    draw_rectangle(w - width, h - height, w + width, h + height, MENU_BG, 1);
    draw_line(w - width, h - height,
        w - width, h + height,
        COLOR_BLACK, MENU_BG, '|');
    draw_line(w + width, h - height,
        w + width, h + height,
        COLOR_BLACK, MENU_BG, '|');
    draw_line(w - width, h + height,
        w + width, h + height,
        COLOR_BLACK, MENU_BG, '=');
    draw_line(w - width, h - height,
        w + width, h - height,
        COLOR_BLACK, MENU_BG, '=');
    const int len = (int)strlen(menu->title);
    draw_text(w - (len>>2), h - height, menu->title, COLOR_BLACK, MENU_BG, len % 2 == 0);
}