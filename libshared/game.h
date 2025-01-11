#ifndef GAME_H
#define GAME_H

#include "../libstructures/sll.h"

typedef struct player {
    char name[20];
    int id;
    int score;
} player;

typedef struct game {
    size_t time;
    sll players;
    _Bool started;
    pthread_mutex_t mutex;
    int ready_count;
} game;

void game_init(game* game);
void add_player(game* game, player* p);
void remove_player(game* game, int player_id);
void game_start(game* game, int time);
void game_stop(game* game);
void game_free(game* game);
void game_clear(game* game);
void game_update_time(game* game, size_t new_time);
void game_update_score(game* game, int player_id, int new_score);

#endif
