#ifndef DRAW_H
#define DRAW_H

void graphics_init();
void graphics_destroy();
void graphics_refresh();

void draw_pixel(int x, int y);
void draw_line(int x1, int y1, int x2, int y2);

#endif
