#include "message_menu.h"
#include <stdlib.h>

#include "../../libdraw/draw.h"
#include "../../libshared/termbox2.h"
#include "../graphics/graphics.h"
#include "renderer.h"

typedef struct message_menu_data {
    void(*callback)(void*);
    void* context;
    char* text;
} message_menu_data;

void message_menu_input(menu* menu, int key, int ch) {
    message_menu_data* data = menu->data;
    switch (key) {
        case TB_KEY_ENTER:
        case TB_KEY_ESC:
            if (data->callback != NULL) data->callback(data->context);
            break;
        default:
            break;
    }
}

#define MESSAGE_MENU_HEIGHT 4
#define MESSAGE_MENU_WIDTH 8

void message_menu_renderer(graphics_context* context, menu* m) {
    message_menu_data* data = m->data;
    render_frame(m, MESSAGE_MENU_WIDTH, MESSAGE_MENU_HEIGHT);
    int w = get_width()>>2, h = get_height()>>1;
    int len = (int)strlen(data->text);
    draw_text(w - (len>>2), (h - MESSAGE_MENU_HEIGHT + 2), data->text, COLOR_RED, MENU_BG, len % 2 == 0);

    draw_text(w-1, (h + MESSAGE_MENU_HEIGHT - 2), "  OK  ", COLOR_RED | COLOR_REVERSE, (MENU_BG + 2) | COLOR_REVERSE, 0);
}

void message_menu_destroy(menu* m) {
    free(m->data);
    free(m);
}

void message_menu_init(char* text, menu* menu, void(*callback)(void*), void* context) {
    menu->type = MENU_TYPE_MESSAGE;
    message_menu_data* data = malloc(sizeof(message_menu_data));
    data->callback = callback;
    data->context = context;
    data->text = text;
    menu->data = data;
    menu->renderer = message_menu_renderer;
    menu->free_func = message_menu_destroy;
    menu->on_key = message_menu_input;
}
