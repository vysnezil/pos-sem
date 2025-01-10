#include "ui.h"

#include "menu/menu.h"
#include "menu/basic_menu.h"
#include "menu/input_menu.h"
#include "../libconnection/socket/socket_connection.h"

#define PORT_DEFAULT 28565

void connect_callback(char* data, void* context) {
    main_context* c = (main_context*)context;
    graphics_context* g = &c->graphics;
    if (data == NULL) {
        show_main_menu(c);
    }
    else {
        size_t len = strlen(data);
        int port = PORT_DEFAULT;
        for (int i = 0; i < len; ++i) {
            if (data[i] == ':') {
                data[i] = '\0';
                port = atoi(data + i + 1);
                break;
            }
        }
        int res = socket_connect(&c->connection, data, port, c->on_receive, context);
        menu_hide(g);
        if (res) {
            // TODO: handle err
            show_main_menu(c);
        }
    }
}

void action_connect(void* data) {
    menu* in = malloc(sizeof(menu));
    in->title = " Connect to the server ";
    input_menu_init("Enter ip hostname:[port]", in, connect_callback, data);
    menu_show(&((main_context*)data)->graphics, in);
}

void action_exit(void* arg) {
    ((main_context*)arg)->running = 0;
}

void action_hide(void* data) {
    menu_hide(&((main_context*)data)->graphics);
    ((main_context*)data)->running = 0;
}

void show_main_menu(main_context* context) {
    menu* m = malloc(sizeof(menu));
    m->type = MENU_TYPE_NOT_INIT;
    m->title = " Main Menu ";
    menu_option* opt = malloc(sizeof(menu_option));
    menu_option* opt2 = malloc(sizeof(menu_option));
    menu_option* opt3 = malloc(sizeof(menu_option));
    *opt = (menu_option){"  Connect to game  ", 1, action_connect, context};
    *opt2 = (menu_option){"    Create game    ", 1, NULL, context};
    *opt3 = (menu_option){"       Exit        ", 1, action_exit, context};
    basic_menu_init(m, 3, opt, opt2, opt3);
    menu_show(&context->graphics, m);
}