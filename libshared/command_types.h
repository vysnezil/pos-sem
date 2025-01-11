#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H

#define COMMAND_INIT 0
#define COMMAND_START 1
#define COMMAND_TIME 2
#define COMMAND_PLAYER 3
#define COMMAND_SCORE 4
#define COMMAND_END 5
#define COMMAND_CIRCLE 6
#define COMMAND_HIT 7
#define COMMAND_PLAYER_READY 7

typedef struct command {
    int type;
    void* data;
} command;

typedef struct command_start_data {
    int time;
} command_start_data;

typedef struct command_time_data {
    int time;
} command_time_data;

typedef struct command_player_data {
    int player_id;
    char name[20];
} command_player_data;

typedef struct command_score_data {
    int score, player_id;
} command_score_data;

typedef struct command_circle_data {
    int id, x, y, r, color;
} command_circle_data;

typedef struct command_hit_data {
    int id;
} command_hit_data;

typedef struct command_ready_data {
    int player_id;
    _Bool ready;
} command_ready_data;

#endif
