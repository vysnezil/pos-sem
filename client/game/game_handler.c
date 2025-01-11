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
    command_player pd = {COMMAND_PLAYER, 0};
    memcpy(&pd.name, data, 20);
    connection_send(&context->connection, &pd, sizeof(command_player));
    menu_hide(&context->graphics);
}

struct on_ready_data {
    main_context* context;
    menu_option* option;
    _Bool ready;
};

void on_ready(void* arg) {
    struct on_ready_data* data = arg;
    main_context* context = data->context;
    data->ready = !data->ready;
    command_ready pd = {COMMAND_PLAYER_READY, 0, data->ready};
    connection_send(&context->connection, &pd, sizeof(command_ready));
    data->option->text = (data->ready) ? "<READY>" : " ready ";
}

void on_leave(void* arg) {
    main_context* context = arg;
    connection_close(&context->connection);
    show_main_menu(context);
}

char* player_tostr(void* obj) {
    return ((player*)obj)->name;
}

void handle_command(void* arg, size_t size, main_context* context) {
    if (size == SIZE_MAX) {
        // TODO: connection lost!
        show_main_menu(context);
        return;
    }
    if (arg == NULL) return;
    game* g = &context->game;
    command_simple* cmd = arg;
    int type =  cmd->type;
    switch (type) {
        case COMMAND_INIT:
            game_init(&context->game);
            menu* in = malloc(sizeof(menu));
            in->title = " Name ";
            input_menu_init("Enter your username", in, name_callback, context);
            menu_show(&context->graphics, in);
            break;
        case COMMAND_START:
            {
                menu_hide(&context->graphics);
                command_start* data = arg;
                game_start(&context->game, data->time);
            }
            break;
        case COMMAND_TIME:
            {
                command_time* data = arg;
                game_update_time(&context->game, data->time);
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
                if (sll_get_size(&g->players) == 0) {
                    menu* l = malloc(sizeof(menu));
                    l->type = MENU_TYPE_NOT_INIT;
                    l->title = " Lobby ";
                    list_menu_init(l, "Connected players", &g->players, &g->mutex, player_tostr);
                    menu_option* op1 = malloc(sizeof(menu_option));
                    menu_option* op2 = malloc(sizeof(menu_option));
                    struct on_ready_data* ready_d = malloc(sizeof(struct on_ready_data));
                    ready_d->context = context;
                    ready_d->option = op1;
                    *op1 = (menu_option) {" ready ", 1, on_ready, ready_d};
                    *op2 = (menu_option) {" leave ", 1, on_leave, context};
                    list_init_options(l, 2, op1, op2);
                    menu_show(&context->graphics, l);
                }
                add_player(&context->game, &p);
            }
            graphics_refresh(&context->graphics, &context->objects);
        }
        break;
        case COMMAND_SCORE:
        {
            command_score* data = arg;
            game_update_score(&context->game, data->player_id, data->score);
        }
        break;
        case COMMAND_CIRCLE:
        {
            command_circle* data = arg;
            object* c = malloc(sizeof(object));
            circle_init(c, data->id, data->x, data->y, data->color, data->r, on_circle, context);
            add_object(&context->objects, c);
        }
        break;
        case COMMAND_END:
        {
            game_stop(&context->game);
            // TODO: show leaderboard
            menu* m = malloc(sizeof(menu));
            m->title = " GAME ENDED!!! ";
            input_menu_init("finally", m, NULL, NULL);
            menu_show(&context->graphics, m);
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