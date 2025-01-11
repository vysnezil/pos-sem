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

typedef struct command_simple {
    int type;
} command_simple;

typedef struct command_start {
    int type;
    int time;
} command_start;

typedef struct command_time {
    int type;
    int time;
} command_time;

typedef struct command_player {
    int type;
    int player_id;
    _Bool disconnect;
    char name[20];
} command_player;

typedef struct command_score {
    int type;
    int score, player_id;
} command_score;

typedef struct command_circle {
    int type;
    int id, x, y, r, color;
} command_circle;

typedef struct command_hit {
    int type;
    int id;
} command_hit;

typedef struct command_ready {
    int type;
    int player_id;
    _Bool ready;
} command_ready;

#endif
