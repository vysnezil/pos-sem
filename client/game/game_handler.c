#include "game_handler.h"

#include "../../libconnection/connection.h"
#include "../../libshared/command_types.h"
#include "../graphics/objects/circle.h"
#include "../menu/basic_menu.h"
#include "../menu/input_menu.h"
#include "../menu/list_menu.h"
#include "../ui.h"

void on_circle(object* obj, void* context_arg) {
    main_context* context = context_arg;
    obj->color |= COLOR_BRIGHT;
    command_hit cmd = (command_hit){ COMMAND_HIT, obj->id };
    connection_send(&context->connection, &cmd, sizeof(command_hit));
}

void name_callback(char* data, void* arg_context) {
    main_context* context = arg_context;
    if (data == NULL) {
        connection_close(&context->connection);
        show_main_menu(context);
        return;
    }
    if (!*data) return;
    command_player pd = {COMMAND_PLAYER, 0 };
    size_t len = strlen(data);
    memcpy(pd.name, data, len > 20 ? 20 : len);
    connection_send(&context->connection, &pd, sizeof(command_player));
    menu_hide(&context->graphics);
}

struct on_ready_opt {
    menu_option option;
    main_context* context;
    _Bool ready;
};

void on_ready(void* arg) {
    struct on_ready_opt* data = arg;
    main_context* context = data->context;
    data->ready = !data->ready;
    command_ready pd = (command_ready){ COMMAND_PLAYER_READY, 0,  data->ready };
    connection_send(&context->connection, &pd, sizeof(command_ready));
    data->option.text = (data->ready) ? "<READY>" : " ready ";
}

void on_leave(void* arg) {
    main_context* context = arg;
    connection_close(&context->connection);
    show_main_menu(context);
}

char* player_tostr(void* obj) {
    return ((player*)obj)->name;
}

char* playerscore_tostr(void* obj) {
    player* p = obj;
    char* buffer = malloc(40 * sizeof(char));
    sprintf(buffer, "%s - %d", p->name, p->score);
    p->tmp_str = buffer;
    return buffer;
}

void show_lobby(void* arg) {
    main_context* context = arg;
    menu* l = malloc(sizeof(menu));
    l->type = MENU_TYPE_NOT_INIT;
    l->title = " Lobby ";
    list_menu_init(l, "Connected players", &context->game->players, &context->game->mutex, player_tostr);
    struct on_ready_opt* op1 = malloc(sizeof(struct on_ready_opt));
    menu_option* op2 = malloc(sizeof(menu_option));
    op1->context = context;
    op1->option = op1->option;
    op1->ready = 0;
    op1->option = (menu_option) {" ready ", 1, on_ready, op1};
    *op2 = (menu_option) {" leave ", 1, on_leave, context};
    list_init_options(l, 2, op1, op2);
    menu_show(&context->graphics, l);
}

void handle_command(void* arg, size_t size, main_context* context) {
    if (size == SIZE_MAX) {
        // TODO: connection lost!
        show_main_menu(context);
        return;
    }
    if (arg == NULL) return;
    game* g = context->game;
    command_simple* cmd = arg;
    int type =  cmd->type;
    switch (type) {
        case COMMAND_INIT:
            context->game = malloc(sizeof(game));
            game_init(context->game);
            menu* in = malloc(sizeof(menu));
            in->title = " Name ";
            input_menu_init("Enter your username", in, name_callback, context);
            menu_show(&context->graphics, in);
            break;
        case COMMAND_START:
            {
                game_clear(g);
                menu_hide(&context->graphics);
                command_start* data = arg;
                game_start(context->game, data->time);
            }
            break;
        case COMMAND_TIME:
            {
                command_time* data = arg;
                game_update_time(context->game, data->time);
            }
            break;
        case COMMAND_PLAYER:
        {
            command_player* data = arg;
            if (data->disconnect) {
                remove_player(g, data->player_id);
            }
            else {
                player p;
                memcpy(&p.name, data->name, 20);
                p.id = data->player_id;
                p.score = 0;
                p.tmp_str = NULL;
                if (sll_get_size(&g->players) == 0) {
                    show_lobby(context);
                }
                add_player(g, &p);
            }
            graphics_refresh(&context->graphics, &context->objects);
        }
        break;
        case COMMAND_SCORE:
        {
            command_score* data = arg;
            game_update_score(g, data->player_id, data->score);
        }
        break;
        case COMMAND_CIRCLE:
        {
            command_circle* data = arg;
            object c;
            circle_init(&c, data->id, data->x, data->y, data->color, data->r, on_circle, context);
            add_object(&context->objects, &c);
        }
        break;
        case COMMAND_END:
        {
            game_stop(g);
            objects_clear(&context->objects);
            menu* l = malloc(sizeof(menu));
            l->type = MENU_TYPE_NOT_INIT;
            l->title = " Results ";
            list_menu_init(l, "Player scores:", &g->players, &g->mutex, playerscore_tostr);
            menu_option* op1 = malloc(sizeof(menu_option));
            *op1 = (menu_option) {"  OK  ", 1, show_lobby, context};
            list_init_options(l, 1, op1);
            menu_show(&context->graphics, l);
        }
        break;
        case COMMAND_HIT:
        {
            command_hit* data = arg;
            remove_object(&context->objects, data->id);
        }
        break;
        default:
            break;
    }
}