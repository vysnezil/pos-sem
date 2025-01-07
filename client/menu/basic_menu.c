#include "basic_menu.h"
#include <stdarg.h>
#include <stdlib.h>
#include "../../libinput/input.h"
#include "renderer.h"

void basic_menu_input(menu* menu, int key, int ch) {
    basic_menu_data* data = menu->data;
    switch (key) {
        case KEY_UP:
            data->selected++;
            if (data->selected >= data->option_count) data->selected = 0;
        break;
        case KEY_DOWN:
            data->selected--;
            if (data->selected < 0) data->selected = data->option_count - 1;
        break;
        case KEY_ENTER:
            if (data->options[data->selected]->on_select != NULL)
                data->options[data->selected]->on_select(data->options[data->selected]->data);
        break;
        default:
            break;
    }
}

void basic_menu_destroy(menu* m) {
    basic_menu_data* data = m->data;
    free(data->options);
    data->options = NULL;
    free(data);
    m->data = NULL;
}

void basic_menu_init(menu* menu, int option_count, ...) {
    if (menu->type != MENU_TYPE_NOT_INIT) return;
    menu->type = MENU_TYPE_BASIC;
    menu->renderer = basic_menu_renderer;
    menu->free_func = basic_menu_destroy;
    menu->on_key = basic_menu_input;
    basic_menu_data* data = malloc(sizeof(basic_menu_data));
    menu->data = data;

    data->option_count = option_count;
    va_list arg;
    va_start(arg, option_count);
    data->options = calloc(option_count, sizeof(menu_option));
    for (int i = 0; i < option_count; i++) {
        data->options[i] = va_arg(arg, menu_option*);
    }
    va_end(arg);
    data->selected = 0;
}

void basic_menu_renderer(graphics_context* context, menu* m) {
    int w = get_width()>>2, h = get_height()>>1;
    draw_rectangle(w - MENU_WIDTH, h - MENU_HEIGHT, w + MENU_WIDTH, h + MENU_HEIGHT, MENU_BG, 1);
    draw_line(w - MENU_WIDTH, h - MENU_HEIGHT,
        w - MENU_WIDTH, h + MENU_HEIGHT,
        COLOR_BLACK, MENU_BG, '|');
    draw_line(w + MENU_WIDTH, h - MENU_HEIGHT,
        w + MENU_WIDTH, h + MENU_HEIGHT,
        COLOR_BLACK, MENU_BG, '|');
    draw_line(w - MENU_WIDTH, h + MENU_HEIGHT,
        w + MENU_WIDTH, h + MENU_HEIGHT,
        COLOR_BLACK, MENU_BG, '=');
    draw_line(w - MENU_WIDTH, h - MENU_HEIGHT,
        w + MENU_WIDTH, h - MENU_HEIGHT,
        COLOR_BLACK, MENU_BG, '=');
    const int len = (int)strlen(m->title);
    draw_text(w - (len>>2), h - MENU_HEIGHT, m->title, COLOR_BLACK, MENU_BG, len % 2 == 0);
    basic_menu_data* data = m->data;
    int offset = data->option_count>>1;
    for (int i = 0; i < data->option_count; i++) {
        int l = (int)strlen(data->options[i]->text);
        int b = i == data->selected ? COLOR_REVERSE : 0;
        draw_text(w - (l>>2), h - offset + i, data->options[i]->text,
            COLOR_RED | b, (MENU_BG + 2) | b, l % 2 == 0);
    }
}