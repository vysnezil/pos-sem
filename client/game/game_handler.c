#include "game_handler.h"
#include "../../libshared/command_types.h"
#include "../graphics/objects/circle.h"

void on_circle(object* obj, void* context_arg) {
    main_context* context = context_arg;
    obj->color |= COLOR_BRIGHT;
    command cmd;
    cmd.type = COMMAND_HIT;
    command_hit_data data;
    data.id = obj->id;
    connection_send(&context->connection, &cmd, sizeof(command) + sizeof(data));
}

void handle_command(void* arg, size_t size, main_context* context) {
    if (arg == NULL) {
        // TODO: connection lost!
        return;
    }
    command* cmd = (command*)arg;
    int type =  cmd->type;
    switch (type) {
        case COMMAND_INIT:
            game_init(context->game);
            // TODO: show lobby
            break;
        case COMMAND_START:
            {
                menu_hide(&context->graphics);
                command_start_data* data = cmd->data;
                game_start(context->game, data->time);
            }
            break;
        case COMMAND_TIME:
            {
                command_time_data* data = cmd->data;
                game_update_time(context->game, data->time);
            }
            break;
        case COMMAND_PLAYER:
        {
            command_player_data* data = cmd->data;
            player p;
            p.name = data->name;
            p.id = data->player_id;
            p.score = 0;
            add_player(context->game, &p);
        }
        break;
        case COMMAND_SCORE:
        {
            command_score_data* data = cmd->data;
            game_update_score(context->game, data->player_id, data->score);
        }
        break;
        case COMMAND_CIRCLE:
        {
            command_circle_data* data = cmd->data;
            object* c = malloc(sizeof(object));
            circle_init(c, data->id, data->x, data->y, data->color, data->r, on_circle, context);
            add_object(&context->objects, c);
        }
        break;
        case COMMAND_END:
        {
            game_stop(context->game);
            // TODO: show leaderboard
        }
        break;
        case COMMAND_HIT:
        {
            command_hit_data* data = cmd->data;
            remove_object(&context->objects, data->id);
        }
        break;
        default:
            break;
    }
}