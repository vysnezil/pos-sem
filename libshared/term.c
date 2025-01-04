#include "term.h"
#define TB_IMPL
#include "termbox2.h"

void term_init() {
    tb_init();
}
void term_destroy() {
    tb_shutdown();
}
void term_update() {
    tb_present();
}
void term_draw(int x, int y, int character, short fg_color, short bg_color) {
    tb_set_cell(x, y, character, fg_color, bg_color);
}
void term_wait_key() {
    struct tb_event ev;
    tb_poll_event(&ev);
}