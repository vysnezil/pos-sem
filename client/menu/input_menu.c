#include "input_menu.h"
#include <stdlib.h>

#include "../../libdraw/draw.h"
#include "../../libshared/termbox2.h"
#include "../graphics/graphics.h"
#include "renderer.h"

void input_menu_input(menu* menu, int key, int ch) {
    input_menu_data* data = menu->data;
    if (ch != 0) {
        if (data->len+2 >= data->limit) {
            char* ptr = realloc(data->buffer, data->limit*2 * sizeof(char));
            if (ptr == NULL) return;
            data->buffer = ptr;
            data->limit <<= 1;
        }
        data->buffer[data->len++] = (char)ch;
        data->buffer[data->len] = 0;
    }
    else {
        switch (key) {
            case TB_KEY_ENTER:
                if (data->callback != NULL) data->callback(data->buffer, data->context);
                break;
            default:
                break;
        }
    }
}

void draw_cut_text(int x, int y, int len, char* text, int fg, int bg, _Bool plusone) {
    int l = (int)strlen(text);
    int n = l > len ? len : l;
    for (int i = 0; i < n; i++) {
        draw_char((x*2)+i+plusone, y, text[i], fg, bg);
    }
}

#define INPUT_MENU_HEIGHT 4
#define INPUT_MENU_WIDTH 8

void input_menu_renderer(struct graphics_context* context, menu* m) {
    input_menu_data* data = m->data;
    char* str = data->buffer;
    int len = (int)strlen(str);
    if (len > 30*INPUT_MENU_HEIGHT) {
        str += len%(30*INPUT_MENU_HEIGHT);
    }
    render_frame(m, 8, INPUT_MENU_HEIGHT);
    int w = get_width()>>2, h = get_height()>>1;
    int prompt_len = (int)strlen(data->prompt);
    draw_text(w - (prompt_len>>2), (h - INPUT_MENU_HEIGHT + 2), data->prompt, COLOR_RED, MENU_BG, prompt_len % 2 == 0);
    for (int i = 0; i <= (len/30 > 3 ? 3 : len/30); i++) {
        draw_line(w-INPUT_MENU_WIDTH+1, (h - INPUT_MENU_HEIGHT + 4) + i, w+7, (h - INPUT_MENU_HEIGHT + 4) + i, COLOR_WHITE, MENU_BG + 2, ' ');
        int offset = (len/30 == 0) ? (len>>2) : INPUT_MENU_WIDTH-1;
        draw_cut_text(w - offset, (h - INPUT_MENU_HEIGHT + 4) + i, 30, str + 30*i, COLOR_RED, MENU_BG + 2, 0);
    }
    if (len > 30*INPUT_MENU_HEIGHT) {
        draw_text((w - INPUT_MENU_WIDTH + 1), (h - INPUT_MENU_HEIGHT + 4),
            "...", COLOR_RED, MENU_BG + 2, 0);
    }
}

void input_menu_destroy(menu* m) {
    input_menu_data* data = m->data;
    if (data->buffer != NULL) free(data->buffer);
    free(m->data);
}

void input_menu_init(char* prompt, menu* menu, void(*callback)(char*, void*), void* context) {
    menu->type = MENU_TYPE_INPUT;
    input_menu_data* data = malloc(sizeof(input_menu_data));
    data->callback = callback;
    data->context = context;
    data->buffer = NULL;
    data->len = 0;
    data->limit = 8;
    data->buffer = calloc(data->limit, sizeof(char));
    data->prompt = prompt;
    menu->data = data;
    menu->renderer = input_menu_renderer;
    menu->free_func = input_menu_destroy;
    menu->on_key = input_menu_input;
}
