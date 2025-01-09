#ifndef DRAW_H
#define DRAW_H

void draw_pixel(int x, int y, int color);
void draw_char(int x, int y, int character, int fg_color, int bg_color);
void draw_line(int x1, int y1, int x2, int y2, int fg_color, int bg_color, int ch);
void draw_circle(int x, int y, int r, void(*draw)(int x, int y, void*), void* context);
void draw_rectangle(int x1, int y1, int x2, int y2, int color, _Bool fill);

void draw_text(int x, int y, char *text, int fg_color, int bg_color, _Bool plusone);

void draw_init();
void draw_destroy();
void draw_update();

int draw_get_width();
int draw_get_height();

#endif