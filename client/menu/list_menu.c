#include "list_menu.h"
#include <stdarg.h>
#include <stdlib.h>
#include "../../libinput/input.h"
#include "renderer.h"
#include <stdatomic.h>

typedef struct list_menu_data {
    sll* list;
    pthread_mutex_t* list_mut;
    char* title;
    char*(*fun)(void* obj);
    int option_count;
    menu_option** options;
    atomic_uint selected;
} list_menu_data;

void list_menu_input(menu* menu, int key, int ch) {
    list_menu_data* data = menu->data;
    switch (key) {
        case KEY_UP:
            ++data->selected;
            data->selected %= data->option_count;
        break;
        case KEY_DOWN:
            ++data->selected;
            data->selected %= data->option_count;
        break;
        case KEY_ENTER:
            if (data->options == NULL) break;
            if (data->options[data->selected]->on_select != NULL)
                data->options[data->selected]->on_select(data->options[data->selected]->data);
        break;
        default:
            break;
    }
}

void list_menu_destroy(menu* m) {
    list_menu_data* data = m->data;
    if (data->option_count > 0) {
        for (int i = 0; i < data->option_count; i++) free(data->options[i]);
        free(data->options);
    }
    free(m->data);
    free(m);
}

struct forach_context {
    int index;
    int* w, *h;
    char*(*fun)(void* obj);
};

void entry_foreach(void* obj_arg, void* data_arg) {
    struct forach_context* context = data_arg;
    int w = get_width()>>2, h = get_height()>>1;
    char* str = context->fun(obj_arg);
    int l = (int)strlen(str);
    draw_text(w - (l>>2), h + context->index++, str, COLOR_RED, MENU_BG, l % 2 == 0);
}

void list_menu_renderer(graphics_context* context, menu* m) {
    int w = get_width()>>2, h = get_height()>>1;
    render_frame(m, LIST_MENU_WIDTH, LIST_MENU_HEIGHT);
    list_menu_data* data = m->data;
    pthread_mutex_lock(data->list_mut);

    int t_len = (int)strlen(data->title);
    draw_text(w - (t_len>>2), (h - LIST_MENU_HEIGHT + 2), data->title, COLOR_RED, MENU_BG, t_len % 2 == 0);
    struct forach_context con;
    con.index = 0;
    con.fun = data->fun;
    con.w = &w;
    con.h = &h;
    sll_for_each(data->list, entry_foreach, &con);
    pthread_mutex_unlock(data->list_mut);

    for (int i = 0; i < data->option_count; i++) {
        menu_option* option = data->options[i];
        int l = (int)strlen(option->text);
        int bg_color = MENU_BG;
        if (option->selectable) bg_color = i == data->selected ? bg_color | COLOR_REVERSE : bg_color + 2;
        draw_text(w - (l>>2), (h + LIST_MENU_HEIGHT - 1 - data->option_count) + i, option->text,
            COLOR_RED, bg_color, l % 2 == 0);
    }
}

void list_menu_init(menu* menu, char* title, sll* list, pthread_mutex_t* mut, char*(*fun)(void* obj)) {
    if (menu->type != MENU_TYPE_NOT_INIT) return;
    menu->type = MENU_TYPE_LIST;
    menu->renderer = list_menu_renderer;
    menu->free_func = list_menu_destroy;
    menu->on_key = list_menu_input;
    list_menu_data* data = malloc(sizeof(list_menu_data));
    menu->data = data;
    data->title = title;
    data->list_mut = mut;
    data->fun = fun;
    data->list = list;
}

void list_init_options(menu* menu, int count, ...) {
    list_menu_data* data = menu->data;
    data->option_count = count;
    va_list arg;
    va_start(arg, count);
    data->options = calloc(count, sizeof(menu_option));
    for (int i = 0; i < count; i++) {
        data->options[i] = va_arg(arg, menu_option*);
    }
    va_end(arg);
    data->selected = 0;
}