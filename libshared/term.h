#ifndef TERM_H
#define TERM_H

void term_init();
void term_destroy();
void term_update();
void term_draw(int x, int y, int character, short fg_color, short bg_color);
void term_wait_key();

#endif

