#ifndef GRAPHICS_DRAW_H
#define GRAPHICS_DRAW_H

void draw_pixel(int x, int y, int color);
void draw_char(int x, int y, int character, int color);
void draw_line(int x1, int y1, int x2, int y2, int color);
void draw_circle(int x, int y, int r, int color);

#endif