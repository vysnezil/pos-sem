#include "basic_menu.h"
#include <stdarg.h>
#include <stdlib.h>
#include "../../libinput/input.h"
#include "renderer.h"

typedef struct basic_menu_data {
    int option_count;
    menu_option** options;
    atomic_uint selected;
} basic_menu_data;

void basic_menu_input(menu* menu, int key, int ch) {
    basic_menu_data* data = menu->data;
    switch (key) {
        case KEY_UP:
            while (data->options[(++data->selected)%data->option_count]->selectable == 0) { }
            data->selected %= data->option_count;
        break;
        case KEY_DOWN:
            while (data->options[(--data->selected)%data->option_count]->selectable == 0) { }
            data->selected %= data->option_count;
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
    for (int i = 0; i < data->option_count; i++) free(data->options[i]);
    free(data->options);
    data->options = NULL;
    free(data);
    free(m);
}

void basic_menu_renderer(graphics_context* context, menu* m) {
    int w = get_width()>>2, h = get_height()>>1;
    render_frame(m, MENU_WIDTH, MENU_HEIGHT);
    basic_menu_data* data = m->data;
    int offset = data->option_count>>1;
    for (int i = 0; i < data->option_count; i++) {
        menu_option* option = data->options[i];
        int l = (int)strlen(option->text);
        int bg_color = MENU_BG;
        if (option->selectable) bg_color = i == data->selected ? bg_color | COLOR_REVERSE : bg_color + 2;
        draw_text(w - (l>>2), h - offset + i, option->text,
            COLOR_RED, bg_color, l % 2 == 0);
    }
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