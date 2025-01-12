#include "ui.h"

#include "menu/menu.h"
#include "menu/basic_menu.h"
#include "menu/input_menu.h"
#include "menu/message_menu.h"
#include "../libconnection/socket/socket_connection.h"

#define PORT_DEFAULT 28565

void show_main(void* arg) {
    show_main_menu(arg);
}

void show_err(char* title, char* text, main_context* c) {
    menu* m = malloc(sizeof(menu));
    m->title = title;
    m->type = MENU_TYPE_NOT_INIT;
    message_menu_init(text, m, show_main, c);
    menu_show(&c->graphics, m);
}

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
        char* ip = malloc(strlen(data)+1);
        strcpy(ip, data);
        menu* m = malloc(sizeof(menu));
        m->title = " Connecting ";
        m->type = MENU_TYPE_NOT_INIT;
        message_menu_init("connecting...", m, NULL, NULL);
        menu_show(&c->graphics, m);
        int res = socket_connect(&c->connection, ip, port, c->on_receive, context);
        free(ip);
        if (res) show_err("Error connecting to server", strerror(res), c);
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

typedef struct start_context {
    main_context* c;
    int port;
} start_context;

void action_start_server(char* data, void* arg) {
    start_context* sc = arg;
    main_context* c = sc->c;
    size_t time = 60;
    if (data == NULL) {
        free(sc);
        show_main_menu(c);
        return;
    }
    if (*data != 0) {
        size_t r = atoi(data);
        if (r > 0) time = r;
    }
    int ser = start_server(sc->port, time);
    if (ser) show_err("Error starting server", strerror(ser), c);
    else {
        int res = socket_connect(&c->connection, "127.0.0.1", sc->port, c->on_receive, c);
        if (res) show_err("Error connecting to server", strerror(res), c);
        else menu_hide(&c->graphics);
    }
    free(sc);
}

void action_ask_time(char* data, void* arg) {
    start_context* sc = malloc(sizeof(start_context));
    short port = 28565;
    if (data == NULL) {
        show_main_menu(arg);
        return;
    }
    if (*data != 0) {
        short r = atoi(data) & 0xFFFF;
        if (r > 0) port = r;
    }
    sc->c = arg;
    sc->port = port;
    menu* in = malloc(sizeof(menu));
    in->title = " Create server ";
    input_menu_init("Enter round time (s): [60]", in, action_start_server, sc);
    menu_show(&((main_context*)arg)->graphics, in);
}

void action_ask_port(void* arg) {
    menu* in = malloc(sizeof(menu));
    in->title = " Create server ";
    input_menu_init("Enter port: [28565]", in, action_ask_time, arg);
    menu_show(&((main_context*)arg)->graphics, in);
}

void show_main_menu(main_context* context) {
    menu* m = malloc(sizeof(menu));
    m->type = MENU_TYPE_NOT_INIT;
    m->title = " Main Menu ";
    menu_option* opt = malloc(sizeof(menu_option));
    menu_option* opt2 = malloc(sizeof(menu_option));
    menu_option* opt3 = malloc(sizeof(menu_option));
    *opt = (menu_option){"  Connect to game  ", 1, action_connect, context};
    *opt2 = (menu_option){"    Create game    ", 1, action_ask_port, context};
    *opt3 = (menu_option){"       Exit        ", 1, action_exit, context};
    basic_menu_init(m, 3, opt, opt2, opt3);
    menu_show(&context->graphics, m);
}